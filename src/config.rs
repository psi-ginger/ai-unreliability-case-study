#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum EndpointMode {
    Local,
    Ngrok,
}

#[derive(Debug, Clone)]
pub struct Endpoints {
    pub front: &'static str,
    pub game: &'static str,
}

impl EndpointMode {
    pub fn endpoints(self) -> Endpoints {
        match self {
            Self::Local => Endpoints {
                front: "127.0.0.1:5700",
                game: "127.0.0.1:4010",
            },
            Self::Ngrok => Endpoints {
                front: "0.tcp.ap.ngrok.io:11853",
                game: "0.tcp.ap.ngrok.io:27911",
            },
        }
    }
}
