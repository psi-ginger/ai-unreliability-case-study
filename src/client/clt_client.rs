use crate::char::char_client::CltCharClient;
use crate::client::state_box::{CltStateBox, UserState};
use crate::config::EndpointMode;
use crate::network::net_session::NetSession;
use crate::world::world_list::CltWorldList;

pub struct CltClient {
    pub user_state: CltStateBox,
    pub world_list: CltWorldList,
    pub my_char: CltCharClient,
    pub front_session: Option<NetSession>,
    pub game_session: Option<NetSession>,
    pub account_id: String,
    pub password: String,
    pub config: EndpointMode,
}

impl CltClient {
    pub fn new(config: EndpointMode, account_id: String, password: String) -> Self {
        Self {
            user_state: CltStateBox::new(),
            world_list: CltWorldList::new(),
            my_char: CltCharClient::new(),
            front_session: None,
            game_session: None,
            account_id,
            password,
            config,
        }
    }

    pub fn state(&self) -> UserState {
        self.user_state.get_state()
    }

    pub fn set_state(&self, state: UserState) {
        self.user_state.set_state(state);
    }
}

impl Default for CltClient {
    fn default() -> Self {
        Self::new(EndpointMode::Local, String::new(), String::new())
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn instantiates() {
        let c = CltClient::default();
        assert_eq!(c.state(), UserState::Disconnected);
        assert_eq!(c.world_list.get_count(), 0);
        assert!(c.my_char.basic_info.is_none());
        assert!(c.front_session.is_none());
        assert!(c.game_session.is_none());
    }
}
