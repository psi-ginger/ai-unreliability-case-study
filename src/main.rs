mod client;
mod config;
mod network;

use clap::Parser;
use config::EndpointMode;

#[derive(Parser, Debug)]
#[command(name = "goonzu-client", version, about = "GoonZu client (Rust port)")]
struct Args {
    /// Use ngrok remote endpoints instead of localhost.
    #[arg(long, default_value_t = false)]
    ngrok: bool,
}

#[tokio::main]
async fn main() -> anyhow::Result<()> {
    tracing_subscriber::fmt::init();
    let args = Args::parse();
    let mode = if args.ngrok {
        EndpointMode::Ngrok
    } else {
        EndpointMode::Local
    };
    let ep = mode.endpoints();
    tracing::info!(front = ep.front, game = ep.game, "endpoints selected");
    Ok(())
}
