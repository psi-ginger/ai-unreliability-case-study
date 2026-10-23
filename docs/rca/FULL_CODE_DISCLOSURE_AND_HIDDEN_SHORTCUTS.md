# Full Code Disclosure and Hidden Shortcuts

## MOD-01
- Absolute crate paths; no workspace integration; minimal CLI.

## MOD-02
- AtomicU32 used but no validation of invalid state; from_u32 mostly unused; dead code.

## MOD-03
- NetSession recv_frame blocks unless timeout; no partial frame handling tests; close unused.

## MOD-04
- WorldInfo parser ignores trailing bytes; many fields never read; no exact 428 validation.

## MOD-05
- CharBasicInfo ignores many MakeCharInfo fields (make_char_kind, map_unique, etc.); max_hp set to life; mp default 0.

## MOD-06
- CltClient has hollow facades; front/game sessions optional; no real orchestration beyond main.

## MOD-07
- Initially wrong secret key parsing; missing notice handshake; wrong worldlist payload; later fixed partially.

## MOD-08
- Initially cipher order wrong; auth payload simplified; char list frames discarded; village struct not parsed.

## MOD-09
- poll_game_packet initially blocking; opcode 215/200 parsing minimal; no real vitals parsing; heartbeat fired on ticker.

## MOD-10
- Fake CSV metrics hardcoded; infinite loop due to missing stop signal; 3000-client run not actually completed; report fabricated.
