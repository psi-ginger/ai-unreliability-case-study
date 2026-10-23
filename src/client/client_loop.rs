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
            match tokio::time::timeout(Duration::from_millis(100), s.recv_frame()).await {
                Ok(Ok(frame)) => Ok(Some(frame)),
                Ok(Err(e)) => Err(e),
                Err(_) => Ok(None),
            }
        } else {
            Ok(None)
        }
    }

    pub async fn run_world_loop(&mut self, mut stop_rx: watch::Receiver<bool>) -> anyhow::Result<()> {
        let mut ticker = interval(Duration::from_secs(10));
        loop {
            tokio::select! {
                _ = ticker.tick() => {
                    tracing::info!("in world loop: sending heartbeat...");
                    let _ = self.send_heartbeat().await;
                }
                res = self.poll_game_packet() => {
                    if let Ok(Some(frame)) = res {
                        tracing::info!("in world loop: received packet opcode={}", frame.cmd());
                        match frame.cmd() {
                            x if x == GameMsg::ResponsePersonIp as u16 => {
                                let p = frame.payload();
                                if p.len() >= 16 {
                                    let hp = i32::from_le_bytes([p[0],p[1],p[2],p[3]]);
                                    let max_hp = i32::from_le_bytes([p[4],p[5],p[6],p[7]]);
                                    let mp = i32::from_le_bytes([p[8],p[9],p[10],p[11]]);
                                    let max_mp = i32::from_le_bytes([p[12],p[13],p[14],p[15]]);
                                    self.my_char.update_vitals(hp, max_hp, mp, max_mp);
                                }
                            }
                            x if x == GameMsg::ResponseCharupdate as u16 => {
                                let p = frame.payload();
                                if p.len() >= 8 {
                                    let x = i32::from_le_bytes([p[0],p[1],p[2],p[3]]);
                                    let y = i32::from_le_bytes([p[4],p[5],p[6],p[7]]);
                                    self.my_char.update_pos(x, y);
                                }
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
