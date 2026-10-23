use gz_proto::game::MakeCharInfo;

#[derive(Debug, Clone, Default)]
pub struct CharBasicInfo {
    pub char_id: u32,
    pub name: String,
    pub level: u16,
    pub map_id: u16,
    pub x: i32,
    pub y: i32,
    pub hp: i32,
    pub max_hp: i32,
    pub mp: i32,
    pub max_mp: i32,
}

#[derive(Debug, Clone, Default)]
pub struct CltCharClient {
    pub basic_info: Option<CharBasicInfo>,
    pub selected_slot: usize,
}

impl CltCharClient {
    pub fn new() -> Self {
        Self::default()
    }

    pub fn init_from_make_char_info(&mut self, info: &MakeCharInfo) {
        self.basic_info = Some(CharBasicInfo {
            char_id: info.make_char_unique as u32,
            name: String::new(),
            level: info.level as u16,
            map_id: info.map_index,
            x: info.x as i32,
            y: info.y as i32,
            hp: info.life as i32,
            max_hp: info.life as i32,
            mp: 0,
            max_mp: 0,
        });
    }

    pub fn update_pos(&mut self, x: i32, y: i32) {
        if let Some(b) = &mut self.basic_info {
            b.x = x;
            b.y = y;
        }
    }

    pub fn update_vitals(&mut self, hp: i32, max_hp: i32, mp: i32, max_mp: i32) {
        if let Some(b) = &mut self.basic_info {
            b.hp = hp;
            b.max_hp = max_hp;
            b.mp = mp;
            b.max_mp = max_mp;
        }
    }

    pub fn update_level(&mut self, level: u16) {
        if let Some(b) = &mut self.basic_info {
            b.level = level;
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn init_and_updates() {
        let info = MakeCharInfo {
            make_char_unique: 5,
            make_char_kind: 1,
            level: 10,
            life: 100,
            x: 3,
            y: 4,
            map_index: 7,
            total_move_speed: 0,
            home_village_unique: 0,
            game_master_level: 0,
            map_unique: 0,
            make_ani_type: 0,
        };
        let mut c = CltCharClient::new();
        c.init_from_make_char_info(&info);
        assert_eq!(c.basic_info.as_ref().unwrap().char_id, 5);
        c.update_pos(9, 9);
        assert_eq!(c.basic_info.as_ref().unwrap().x, 9);
        c.update_vitals(50, 100, 20, 50);
        assert_eq!(c.basic_info.as_ref().unwrap().hp, 50);
        c.update_level(11);
        assert_eq!(c.basic_info.as_ref().unwrap().level, 11);
    }
}
