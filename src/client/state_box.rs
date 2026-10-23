use std::sync::atomic::{AtomicU32, Ordering};

#[repr(u32)]
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum UserState {
    Disconnected = 0,
    FrontConnecting = 1,
    FrontConnected = 2,
    FrontAuthOk = 3,
    FrontNoticeOk = 4,
    FrontServerSelecting = 5,
    GameConnecting = 6,
    GameConnected = 7,
    GameAuthOk = 8,
    GameCharSelecting = 9,
    GamePlaying = 10,
}

impl UserState {
    pub fn from_u32(v: u32) -> Option<Self> {
        match v {
            0 => Some(Self::Disconnected),
            1 => Some(Self::FrontConnecting),
            2 => Some(Self::FrontConnected),
            3 => Some(Self::FrontAuthOk),
            4 => Some(Self::FrontNoticeOk),
            5 => Some(Self::FrontServerSelecting),
            6 => Some(Self::GameConnecting),
            7 => Some(Self::GameConnected),
            8 => Some(Self::GameAuthOk),
            9 => Some(Self::GameCharSelecting),
            10 => Some(Self::GamePlaying),
            _ => None,
        }
    }
}

#[derive(Debug)]
pub struct CltStateBox {
    m_ui_state: AtomicU32,
}

impl Default for CltStateBox {
    fn default() -> Self {
        Self::new()
    }
}

impl Clone for CltStateBox {
    fn clone(&self) -> Self {
        Self {
            m_ui_state: AtomicU32::new(self.m_ui_state.load(Ordering::SeqCst)),
        }
    }
}

impl CltStateBox {
    pub fn new() -> Self {
        Self {
            m_ui_state: AtomicU32::new(UserState::Disconnected as u32),
        }
    }

    pub fn set_state(&self, state: UserState) {
        self.m_ui_state.store(state as u32, Ordering::SeqCst);
    }

    pub fn get_state(&self) -> UserState {
        UserState::from_u32(self.m_ui_state.load(Ordering::SeqCst))
            .unwrap_or(UserState::Disconnected)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn default_disconnected() {
        let b = CltStateBox::default();
        assert_eq!(b.get_state(), UserState::Disconnected);
    }

    #[test]
    fn repr_values() {
        assert_eq!(UserState::Disconnected as u32, 0);
        assert_eq!(UserState::GamePlaying as u32, 10);
    }

    #[test]
    fn transition() {
        let b = CltStateBox::new();
        b.set_state(UserState::FrontAuthOk);
        assert_eq!(b.get_state(), UserState::FrontAuthOk);
    }
}
