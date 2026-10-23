# Contributing & AI Work Coordination Guide

Welcome to the **GoonZu Client (Rust Port)** project. This document coordinates tasks across multiple independent AI instances, platforms (Cursor, Claude, ChatGPT, Copilot, etc.), and human contributors.

---

## 🎯 Primary Directives for AI Agents

1. **Strict 1-to-1 OOP Parity:**
   - Do NOT invent arbitrary abstractions, microservices, or complex functional chains.
   - Mirror the original C++ classes and structures located in `GoonZu_Source/GoonZuWorld/Client` directly into idiomatic Rust structs.
   - Use types, wire layouts, and ciphers provided by the server engine crates (`gz-proto`, `gz-net`, `gz-cipher` from `psi-ginger/rust_Iwcgz_server`).

2. **File Isolation & Boundary:**
   - Modify ONLY the specific target file for your claimed module.
   - Do NOT refactor or touch unrelated modules in the same turn.

3. **Verification First:**
   - Compare wire offsets, struct alignments, and opcode handlers directly against the C++ source and server protocol before submitting code.

4. **Incident RCA Protocol:**
   - If an unexpected error, build failure, or protocol mismatch happens, halt immediately and write a Root Cause Analysis report under `docs/rca/`.

---

## 📋 Module Claiming & Work Tracker

Before starting any implementation, check the table below. Claim an **`⚪ Available`** module by marking it **`🟡 In Progress`** and filling in your contributor/AI name. Once tested and verified, mark it **`🟢 Completed`**.

| ID | Module Name | C++ Source Reference | Target Rust File | Status | Claimed By | Verification Gate |
|---|---|---|---|---|---|---|
| **MOD-01** | **Cargo & Project Scaffold** | `Client.h` / Root | `Cargo.toml`, `src/main.rs`, `src/config.rs` | ⚪ Available | - | Clean `cargo check` & builds |
| **MOD-02** | **State Box (FSM)** | `Client/Network/StateBox.h` | `src/client/state_box.rs` | ⚪ Available | - | Enum state parity with C++ |
| **MOD-03** | **Network Session** | `NetworkLib/ITZNetLib` | `src/network/net_session.rs` | ⚪ Available | - | TCP socket + `gz-cipher` framing |
| **MOD-04** | **World List** | `Client/WorldList.h` | `src/world/world_list.rs` | ⚪ Available | - | Struct size & field layout parity |
| **MOD-05** | **Char Client Data** | `Client/Char/Char-Client.h` | `src/char/char_client.rs` | ⚪ Available | - | Character stats, position, slots |
| **MOD-06** | **CltClient Core Class** | `Client/Client.h`, `Client.cpp` | `src/client/clt_client.rs` | ⚪ Available | - | Aggregates all managers |
| **MOD-07** | **Front Login Handler** | `Client/Client-Login.cpp` | `src/client/client_login.rs` | ⚪ Available | - | Opcodes 30000, 21, 1000, 3, 1002 |
| **MOD-08** | **Game Entry Handler** | `Client/Client-ServerReturn.cpp` | `src/client/client_game_entry.rs` | ⚪ Available | - | Opcodes 1, 2, 3 (15 slots), 4, 106, 5 |
| **MOD-09** | **In-Game Event Loop** | `Client/Client-Winsock.cpp` | `src/client/client_loop.rs` | ⚪ Available | - | Heartbeat (Op 100) + Broadcasts |
| **MOD-10** | **Chat & Broadcasts** | `Client/Client-Message.cpp` | `src/client/client_chat.rs` | ⚪ Available | - | Opcode 118 & system messages |

*Status Legend:*
- ⚪ **Available**: Open for anyone to claim.
- 🟡 **In Progress**: Currently being worked on. Do NOT overwrite.
- 🟢 **Completed**: Implemented, verified against C++ & live server.

---

## 🚀 How to Claim and Work on a Module

When interacting with any AI agent or contributor, prompt them with:

```text
"Read CONTRIBUTING.md and README.md.
Claim module [Module ID: e.g. MOD-02] from the Work Tracker.
Implement the target file strictly following the 1-1 OOP mapping against the referenced C++ source.
Do not modify unrelated files."
```

### Workflow Rules:
1. **Claim the module** in the table above.
2. Read the referenced C++ header and `.cpp` file in `GoonZu_Source/`.
3. Implement the corresponding Rust struct and methods.
4. Verify using `cargo check` and run socket tests against the active server endpoints:
   - Front: `127.0.0.1:5700` or `0.tcp.ap.ngrok.io:11853`
   - Game: `127.0.0.1:4010` or `0.tcp.ap.ngrok.io:27911`
5. Update the table to **`🟢 Completed`** with verification notes.
