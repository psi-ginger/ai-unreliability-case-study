mod char;
mod client;
mod config;
mod network;
mod world;

use clap::Parser;
use client::clt_client::CltClient;
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
    tokio::time::sleep(std::time::Duration::from_secs(300)).await;
    for h in handles { let _ = h.await; }
    Ok(())
}
