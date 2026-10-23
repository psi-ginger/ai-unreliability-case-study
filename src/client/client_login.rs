use crate::client::clt_client::CltClient;
use crate::client::state_box::UserState;
use crate::world::world_list::WorldInfo;
use gz_cipher::Cipher;
use gz_net::Frame;
use gz_proto::opcodes_front::FrontMsg;

impl CltClient {
    pub async fn connect_front(&mut self) -> anyhow::Result<()> {
        self.set_state(UserState::FrontConnecting);
        let addr = self.config.endpoints().front;
        let mut session = crate::network::net_session::NetSession::connect(addr).await?;
        self.set_state(UserState::FrontConnected);
        // wait for secret key
        let frame = session.recv_frame().await?;
        if frame.cmd() != FrontMsg::ResponseSecretkey as u16 {
            return Err(anyhow::anyhow!("expected secret key"));
        }
        let payload = frame.payload();
        if payload.len() < 5 {
            return Err(anyhow::anyhow!("short secret key"));
        }
        session.set_out_cipher(Some(Cipher::session_cipher(payload[2], payload[4])));
        // Front sends plaintext inbound frames.
        session.set_in_cipher(None);
        self.front_session = Some(session);
        self.set_state(UserState::FrontConnected);
        Ok(())
    }

    pub async fn login_front(&mut self) -> anyhow::Result<()> {
        let mut w = gz_proto::wire::Writer::new();
        w.fixed_str(&self.account_id, 20);
        w.fixed_str(&self.password, 20);
        w.fixed_str("00-00-00-00-00-00", 20);
        w.pad(2048);
        let body = w.finish();
        let frame = Frame::new(FrontMsg::RequestEnglishLogin as u16, &body)?;
        let session = self.front_session.as_mut().ok_or_else(|| anyhow::anyhow!("no front session"))?;
        session.send_frame(&frame).await?;
        let resp = session.recv_frame().await?;
        if resp.cmd() != 1000 {
            return Err(anyhow::anyhow!("expected login response"));
        }
        let p = resp.payload();
        if p.len() >= 6 {
            self.auth_key = i32::from_le_bytes([p[2], p[3], p[4], p[5]]);
        }
        // notice handshake
        let notice_req = Frame::new(FrontMsg::RequestNotice as u16, &[])?;
        session.send_frame(&notice_req).await?;
        let _notice_resp = session.recv_frame().await?;
        self.set_state(UserState::FrontAuthOk);
        Ok(())
    }

    pub async fn fetch_world_list(&mut self) -> anyhow::Result<()> {
        let mut w = gz_proto::wire::Writer::new();
        w.fixed_str(&self.account_id, 20);
        let body = w.finish();
        let frame = Frame::new(FrontMsg::RequestWorldlist as u16, &body)?;
        let session = self.front_session.as_mut().ok_or_else(|| anyhow::anyhow!("no front session"))?;
        session.send_frame(&frame).await?;
        let resp = session.recv_frame().await?;
        if resp.cmd() != FrontMsg::ResponseWorldlist as u16 {
            return Err(anyhow::anyhow!("expected world list"));
        }
        let body = resp.payload();
        if body.len() < 2 {
            return Err(anyhow::anyhow!("short world list"));
        }
        let count = u16::from_le_bytes([body[0], body[1]]) as usize;
        self.world_list.clear();
        let mut off = 2;
        for _ in 0..count {
            if off + 428 > body.len() {
                break;
            }
            let chunk: [u8; 428] = body[off..off + 428].try_into().unwrap();
            self.world_list.add_world(WorldInfo::from_wire_bytes(&chunk));
            off += 428;
        }
        self.set_state(UserState::FrontServerSelecting);
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn login_frame_len() {
        let body = vec![0u8; 2108];
        let f = Frame::new(FrontMsg::RequestEnglishLogin as u16, &body).unwrap();
        assert_eq!(f.payload().len(), 2108);
    }

    #[test]
    fn world_list_parse() {
        let mut body = vec![0u8; 2 + 428];
        body[0] = 1;
        let w = WorldInfo::from_wire_bytes(&[0u8; 428]);
        // just ensure no panic
        let _ = w;
    }
}
