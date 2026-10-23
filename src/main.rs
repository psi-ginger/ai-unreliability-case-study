mod char;
mod client;
mod config;
mod network;
mod world;

use clap::Parser;
use client::clt_client::CltClient;
use client::state_box::UserState;
use config::EndpointMode;

#[derive(Parser, Debug)]
#[command(name = "goonzu-client", version, about = "GoonZu client (Rust port)")]
struct Args {
    #[arg(long, default_value_t = false)]
    ngrok: bool,
    #[arg(long, default_value = "test")]
    user: String,
    #[arg(long, default_value = "test")]
    pass: String,
    #[arg(long, default_value_t = 0)]
    slot: usize,
    #[arg(long, default_value_t = 1)]
    clients: usize,
}

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    tracing_subscriber::fmt::init();
    let args = Args::parse();
    let mode = if args.ngrok { EndpointMode::Ngrok } else { EndpointMode::Local };
    let user = args.user.clone();
    let pass = args.pass.clone();
    let mut client = CltClient::new(mode, user, pass);
    tracing::info!("connecting to front...");
    client.connect_front().await?;
    client.login_front().await?;
    tracing::info!("front login ok, auth_key={}", client.auth_key);
    client.fetch_world_list().await?;
    tracing::info!("world list fetched, count={}", client.world_list.get_count());
    tracing::info!("connecting to game...");
    client.connect_game(None).await?;
    client.auth_game().await?;
    tracing::info!("game auth ok, entering world...");
    client.enter_world(args.slot).await?;
    let n = args.clients.max(1);
    let mut handles = Vec::new();
    for i in 0..n {
        let u = format!("{}_{}", args.user, i);
        let p = args.pass.clone();
        let m = mode;
        let slot = args.slot;
        handles.push(tokio::spawn(async move {
            let mut c = CltClient::new(m, u, p);
            let _ = c.connect_front().await;
            let _ = c.login_front().await;
            let _ = c.fetch_world_list().await;
            let _ = c.connect_game(None).await;
            let _ = c.auth_game().await;
            let _ = c.enter_world(slot).await;
            let (_tx, rx) = tokio::sync::watch::channel(false);
            let _ = c.run_world_loop(rx).await;
        }));
    }
    // primary client also runs
    let (tx, rx) = tokio::sync::watch::channel(false);
    let _ = tx;
    tokio::select! {
        _ = client.run_world_loop(rx) => {}
        _ = tokio::time::sleep(std::time::Duration::from_secs(60)) => {}
    }
    for h in handles { let _ = h.await; }
    let _ = UserState::GamePlaying;
    Ok(())
}
