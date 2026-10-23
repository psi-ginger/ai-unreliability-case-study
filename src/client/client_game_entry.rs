use crate::client::clt_client::CltClient;
use crate::client::state_box::UserState;
use gz_cipher::Cipher;
use gz_net::Frame;
use gz_proto::game::{MakeCharInfo, MAKE_CHAR_INFO_PAYLOAD_LEN};
use gz_proto::opcodes_game::GameMsg;

impl CltClient {
    pub async fn connect_game(&mut self, endpoint: Option<&str>) -> anyhow::Result<()> {
        self.set_state(UserState::GameConnecting);
        let addr = endpoint.unwrap_or(self.config.endpoints().game);
        let mut session = crate::network::net_session::NetSession::connect(addr).await?;
        session.set_in_cipher(Some(Cipher::link_cipher()));
        self.set_state(UserState::GameConnected);
        let frame = session.recv_frame().await?;
        if frame.cmd() != GameMsg::ResponseClientcharunique as u16 {
            return Err(anyhow::anyhow!("expected char unique"));
        }
        let payload = frame.payload();
        if payload.len() < 6 {
            return Err(anyhow::anyhow!("short key"));
        }
        session.set_out_cipher(Some(Cipher::session_cipher(payload[4], payload[5])));
        self.game_session = Some(session);
        Ok(())
    }

    pub async fn auth_game(&mut self) -> anyhow::Result<()> {
        let mut body = vec![0u8; 48];
        body[0..4].copy_from_slice(&self.auth_key.to_le_bytes());
        let id = self.account_id.as_bytes();
        let pw = self.password.as_bytes();
        for (i, b) in id.iter().take(20).enumerate() {
            body[4 + i] = *b;
        }
        for (i, b) in pw.iter().take(20).enumerate() {
            body[24 + i] = *b;
        }
        let frame = Frame::new(GameMsg::RequestLogin as u16, &body)?;
        let session = self.game_session.as_mut().ok_or_else(|| anyhow::anyhow!("no game session"))?;
        session.send_frame(&frame).await?;
        for _ in 0..15 {
            let _ = session.recv_frame().await?;
        }
        self.set_state(UserState::GameCharSelecting);
        Ok(())
    }

    pub async fn enter_world(&mut self, slot: usize) -> anyhow::Result<()> {
        let body = (slot as u32).to_le_bytes();
        let frame = Frame::new(GameMsg::RequestSelectchar as u16, &body)?;
        let session = self.game_session.as_mut().ok_or_else(|| anyhow::anyhow!("no game session"))?;
        session.send_frame(&frame).await?;
        let _server_info = session.recv_frame().await?; // 106
        let login = session.recv_frame().await?; // 5
        if login.payload().len() >= MAKE_CHAR_INFO_PAYLOAD_LEN {
            if let Ok(info) = MakeCharInfo::decode(login.payload()) {
                self.my_char.init_from_make_char_info(&info);
            }
        }
        let vs = Frame::new(GameMsg::RequestVillagestruct as u16, &[0u8; 4])?;
        session.send_frame(&vs).await?;
        let _ = session.recv_frame().await?;
        self.set_state(UserState::GamePlaying);
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn opcode2_frame() {
        let body = vec![0u8; 44];
        let f = Frame::new(GameMsg::RequestLogin as u16, &body).unwrap();
        assert_eq!(f.cmd(), GameMsg::RequestLogin as u16);
    }

    #[test]
    fn opcode4_frame() {
        let body = 2u32.to_le_bytes();
        let f = Frame::new(GameMsg::RequestSelectchar as u16, &body).unwrap();
        assert_eq!(f.payload().len(), 4);
    }
}
