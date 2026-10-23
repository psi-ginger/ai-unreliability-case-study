use crate::client::clt_client::CltClient;
use gz_net::Frame;
use gz_proto::opcodes_game::GameMsg;
use tokio::sync::watch;
use tokio::time::{interval, Duration};

impl CltClient {
    pub async fn send_heartbeat(&mut self) -> anyhow::Result<()> {
        let frame = Frame::new(GameMsg::RequestHeartbeat as u16, &[])?;
        if let Some(s) = &mut self.game_session {
            s.send_frame(&frame).await?;
        }
        Ok(())
    }

    pub async fn poll_game_packet(&mut self) -> anyhow::Result<Option<Frame>> {
        if let Some(s) = &mut self.game_session {
            Ok(Some(s.recv_frame().await?))
        } else {
            Ok(None)
        }
    }

    pub async fn run_world_loop(&mut self, mut stop_rx: watch::Receiver<bool>) -> anyhow::Result<()> {
        let mut ticker = interval(Duration::from_secs(10));
        loop {
            tokio::select! {
                _ = ticker.tick() => {
                    let _ = self.send_heartbeat().await;
                }
                res = self.poll_game_packet() => {
                    if let Ok(Some(frame)) = res {
                        match frame.cmd() {
                            x if x == GameMsg::ResponsePersonIp as u16 => {
                                // hp/mp update not parsed in detail
                            }
                            x if x == GameMsg::ResponseCharupdate as u16 => {
                                // char update handled elsewhere
                            }
                            _ => {}
                        }
                    }
                }
                changed = stop_rx.changed() => {
                    if changed.is_ok() && *stop_rx.borrow() {
                        break;
                    }
                }
            }
        }
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn heartbeat_frame() {
        let f = Frame::new(GameMsg::RequestHeartbeat as u16, &[]).unwrap();
        assert_eq!(f.cmd(), 100);
    }
}
