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
    #[arg(long, default_value_t = false)]
    dual: bool,
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
    if args.dual {
        let user2 = args.user.clone() + "2";
        let pass2 = args.pass.clone();
        let mode2 = mode;
        let handle = tokio::spawn(async move {
            let mut c2 = CltClient::new(mode2, user2, pass2);
            let _ = c2.connect_front().await;
            let _ = c2.login_front().await;
            let _ = c2.fetch_world_list().await;
            let _ = c2.connect_game(None).await;
            let _ = c2.auth_game().await;
            let _ = c2.enter_world(0).await;
            let (_tx, rx) = tokio::sync::watch::channel(false);
            let _ = c2.run_world_loop(rx).await;
        });
        let (tx, rx) = tokio::sync::watch::channel(false);
        let _ = tx;
        // run for 60s then stop
        tokio::select! {
            _ = client.run_world_loop(rx) => {}
            _ = tokio::time::sleep(std::time::Duration::from_secs(60)) => {}
        }
        let _ = handle.await;
    } else {
        let (tx, rx) = tokio::sync::watch::channel(false);
        let _ = tx;
        client.run_world_loop(rx).await?;
    }
    let _ = UserState::GamePlaying;
    Ok(())
}
