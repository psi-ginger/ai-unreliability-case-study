fn cstr(bytes: &[u8]) -> String {
    let end = bytes.iter().position(|&b| b == 0).unwrap_or(bytes.len());
    String::from_utf8_lossy(&bytes[..end]).to_string()
}

#[derive(Debug, Clone, Default)]
pub struct GameServerBaseInfo {
    pub world_id: u16,
    pub world_name: String,
    pub server_id: i32,
    pub ip: String,
    pub port: u16,
    pub open_date: String,
    pub short_message: String,
}

#[derive(Debug, Clone, Default)]
pub struct GameServerRefreshInfo {
    pub world_id: u16,
    pub state: u16,
    pub current_connections: u16,
}

#[derive(Debug, Clone, Default)]
pub struct GameServerInfo {
    pub goonzu_name: String,
    pub goonzu_text: String,
    pub population: i32,
    pub current_price: i32,
    pub npc_rate: i32,
    pub fixation_money: i32,
}

#[derive(Debug, Clone, Default)]
pub struct WorldInfo {
    pub baseinfo: GameServerBaseInfo,
    pub refreshinfo: GameServerRefreshInfo,
    pub gameserverinfo: GameServerInfo,
}

impl WorldInfo {
    pub fn from_wire_bytes(bytes: &[u8; 428]) -> Self {
        // offsets
        let mut off = 0;
        let world_id = u16::from_le_bytes([bytes[off], bytes[off + 1]]);
        off += 2;
        let world_name = cstr(&bytes[off..off + 20]);
        off += 20;
        let server_id = i32::from_le_bytes([bytes[off], bytes[off + 1], bytes[off + 2], bytes[off + 3]]);
        off += 4;
        let ip = cstr(&bytes[off..off + 16]);
        off += 16;
        let port = u16::from_le_bytes([bytes[off], bytes[off + 1]]);
        off += 2;
        let open_date = cstr(&bytes[off..off + 15]);
        off += 15;
        let short_message = cstr(&bytes[off..off + 64]);
        off += 64;

        let refresh_world_id = u16::from_le_bytes([bytes[off], bytes[off + 1]]);
        off += 2;
        let state = u16::from_le_bytes([bytes[off], bytes[off + 1]]);
        off += 2;
        let current_connections = u16::from_le_bytes([bytes[off], bytes[off + 1]]);
        off += 2;

        let goonzu_name = cstr(&bytes[off..off + 20]);
        off += 20;
        let goonzu_text = cstr(&bytes[off..off + 256]);
        off += 256;
        let population = i32::from_le_bytes([bytes[off], bytes[off + 1], bytes[off + 2], bytes[off + 3]]);
        off += 4;
        let current_price = i32::from_le_bytes([bytes[off], bytes[off + 1], bytes[off + 2], bytes[off + 3]]);
        off += 4;
        let npc_rate = i32::from_le_bytes([bytes[off], bytes[off + 1], bytes[off + 2], bytes[off + 3]]);
        off += 4;
        let fixation_money = i32::from_le_bytes([bytes[off], bytes[off + 1], bytes[off + 2], bytes[off + 3]]);
        // off += 4;

        WorldInfo {
            baseinfo: GameServerBaseInfo {
                world_id,
                world_name,
                server_id,
                ip,
                port,
                open_date,
                short_message,
            },
            refreshinfo: GameServerRefreshInfo {
                world_id: refresh_world_id,
                state,
                current_connections,
            },
            gameserverinfo: GameServerInfo {
                goonzu_name,
                goonzu_text,
                population,
                current_price,
                npc_rate,
                fixation_money,
            },
        }
    }
}

#[derive(Debug, Clone)]
pub struct CltWorldList {
    pub m_worldlist: Vec<WorldInfo>,
    pub m_count: usize,
}

impl Default for CltWorldList {
    fn default() -> Self {
        Self::new()
    }
}

impl CltWorldList {
    pub fn new() -> Self {
        Self {
            m_worldlist: Vec::with_capacity(100),
            m_count: 0,
        }
    }

    pub fn clear(&mut self) {
        self.m_worldlist.clear();
        self.m_count = 0;
    }

    pub fn get_count(&self) -> usize {
        self.m_count
    }

    pub fn add_world(&mut self, info: WorldInfo) {
        if self.m_worldlist.len() < 100 {
            self.m_worldlist.push(info);
            self.m_count = self.m_worldlist.len();
        }
    }

    pub fn get_world_by_id(&self, id: u16) -> Option<&WorldInfo> {
        self.m_worldlist.iter().find(|w| w.baseinfo.world_id == id)
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn parse_dummy() {
        let mut bytes = [0u8; 428];
        // world_id at 0
        bytes[0] = 7;
        bytes[1] = 0;
        // world_name
        let name = b"World7\0";
        bytes[2..2 + name.len()].copy_from_slice(name);
        let w = WorldInfo::from_wire_bytes(&bytes);
        assert_eq!(w.baseinfo.world_id, 7);
        assert_eq!(w.baseinfo.world_name, "World7");
    }

    #[test]
    fn add_query() {
        let mut list = CltWorldList::new();
        let mut wi = WorldInfo::default();
        wi.baseinfo.world_id = 3;
        list.add_world(wi);
        assert_eq!(list.get_count(), 1);
        assert!(list.get_world_by_id(3).is_some());
    }
}
