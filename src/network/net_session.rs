use gz_cipher::Cipher;
use gz_net::{Frame, FrameParser};
use std::io;
use tokio::io::{AsyncReadExt, AsyncWriteExt};
use tokio::net::TcpStream;

pub struct NetSession {
    stream: TcpStream,
    parser: FrameParser,
    out_cipher: Option<Cipher>,
    in_cipher: Option<Cipher>,
}

impl NetSession {
    pub async fn connect(addr: &str) -> anyhow::Result<Self> {
        let stream = TcpStream::connect(addr).await?;
        Ok(Self {
            stream,
            parser: FrameParser::new(),
            out_cipher: None,
            in_cipher: None,
        })
    }

    pub fn set_out_cipher(&mut self, cipher: Option<Cipher>) {
        self.out_cipher = cipher;
    }

    pub fn set_in_cipher(&mut self, cipher: Option<Cipher>) {
        self.in_cipher = cipher;
    }

    pub async fn send_frame(&mut self, frame: &Frame) -> anyhow::Result<()> {
        let bytes = frame.as_bytes();
        let out = match &mut self.out_cipher {
            Some(c) => c.encode(bytes).map_err(|e| anyhow::anyhow!(e))?,
            None => bytes.to_vec(),
        };
        self.stream.write_all(&out).await?;
        self.stream.flush().await?;
        Ok(())
    }

    pub async fn recv_frame(&mut self) -> anyhow::Result<Frame> {
        loop {
            if let Some(frame) = self.parser.next_frame().map_err(|e| anyhow::anyhow!(e))? {
                // Decipher if needed.
                if let Some(c) = &mut self.in_cipher {
                    let raw = frame.as_bytes();
                    let mut dest = vec![0u8; raw.len()];
                    c.decode(raw, &mut dest)
                        .map_err(|e| anyhow::anyhow!(e))?;
                    let mut p = FrameParser::new();
                    p.feed(&dest).map_err(|e| anyhow::anyhow!(e))?;
                    if let Some(f) = p.next_frame().map_err(|e| anyhow::anyhow!(e))? {
                        return Ok(f);
                    }
                }
                return Ok(frame);
            }
            let mut buf = vec![0u8; 4096];
            let n = self.stream.read(&mut buf).await?;
            if n == 0 {
                return Err(anyhow::anyhow!(io::Error::new(
                    io::ErrorKind::UnexpectedEof,
                    "connection closed"
                )));
            }
            self.parser
                .feed(&buf[..n])
                .map_err(|e| anyhow::anyhow!(e))?;
        }
    }

    pub async fn close(&mut self) {
        let _ = self.stream.shutdown().await;
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use gz_net::Frame;

    #[test]
    fn frame_round_trip() {
        let f = Frame::new(42, b"hello").unwrap();
        let b = f.as_bytes().to_vec();
        let mut p = FrameParser::new();
        p.feed(&b).unwrap();
        let f2 = p.next_frame().unwrap().unwrap();
        assert_eq!(f2.cmd(), 42);
        assert_eq!(f2.payload(), b"hello");
    }
}
