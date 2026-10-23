# Contributing & AI Work Coordination Guide

Welcome to the **GoonZu Client (Rust Port)** project. This document coordinates tasks across multiple independent AI instances, remote machines/environments, and human contributors.

---

## 🛡️ Quarantine Branch Protocol (STRICTLY ENFORCED)

> [!CAUTION]
> **NEVER PUSH DIRECTLY TO `main`!**
> The `main` branch is locked and reserved exclusively for verified, working, and QC-approved code. All contributors and external AI instances must work inside dedicated quarantine branches.

### Branching Rules:
1. **Branch Naming Standard:**
   - Every claimed module must be developed on a quarantine branch named:
     `feat/<module-id>-<short-description>` (e.g. `feat/mod-01-scaffold`, `feat/mod-02-statebox`).
2. **Quarantine Isolation:**
   - Check out from the latest `main`:
     ```bash
     git checkout main && git pull origin main
     git checkout -b feat/<module-id>-<short-description>
     ```
3. **Quarantine Push:**
   - Once implementation passes local `cargo check`, push ONLY to your quarantine branch:
     ```bash
     git add .
     git commit -m "feat(<module-id>): implement <description>"
     git push origin feat/<module-id>-<short-description>
     ```
4. **QC Release & Merge Gate:**
   - The Lead QC (local workstation) will pull the quarantine branch, verify 1-1 C++ parity, run socket integration tests, and merge into `main`.

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

Before starting any implementation, check the table below. Claim an **`⚪ Available`** module by marking it **`🟡 In Progress`** and filling in your contributor/AI name and quarantine branch. Once verified and merged into `main`, it will be marked **`🟢 Completed`**.

| ID | Module Name | C++ Source Reference | Target Rust File | Quarantine Branch | Status | Claimed By | Verification Gate |
|---|---|---|---|---|---|---|---|
| **MOD-01** | **Cargo & Project Scaffold** | `Client.h` / Root | `Cargo.toml`, `src/main.rs`, `src/config.rs` | `feat/mod-01-scaffold` | 🟢 Completed | PsiGinger / Agent 1 | Clean `cargo check` & builds verified |
| **MOD-02** | **State Box (FSM)** | Client/Network/StateBox.h | src/client/state_box.rs | 🟢 Completed | feat/mod-02-statebox | cargo check & cargo test passed (3 tests) |
| **MOD-03** | **Network Session** | NetworkLib/ITZNetLib | src/network/net_session.rs | 🟢 Completed | feat/mod-03-net-session | cargo check & cargo test passed (4 tests) |
| **MOD-04** | **World List** | Client/WorldList.h | src/world/world_list.rs | 🟢 Completed | feat/mod-04-world-list | cargo check & cargo test passed (6 tests) |
| **MOD-05** | **Char Client Data** | Client/Char/Char-Client.h | src/char/char_client.rs | 🟢 Completed | feat/mod-05-char-client | cargo check & cargo test passed (7 tests) |
| **MOD-06** | **CltClient Core Class** | Client/Client.h, Client.cpp | src/client/clt_client.rs | 🟢 Completed | feat/mod-06-clt-client | cargo check & cargo test passed (8 tests) |
| **MOD-07** | **Front Login Handler** | Client/Client-Login.cpp | src/client/client_login.rs | 🟢 Completed | feat/mod-07-front-login | cargo check & cargo test passed (10 tests) |
| **MOD-08** | **Game Entry Handler** | Client/Client-ServerReturn.cpp | src/client/client_game_entry.rs | 🟢 Completed | feat/mod-08-game-entry | cargo check & cargo test passed (12 tests) |
| **MOD-09** | **In-Game Event Loop** | Client/Client-Winsock.cpp | src/client/client_loop.rs | 🟢 Completed | feat/mod-09-in-game-loop | cargo check & cargo test passed (13 tests) |
| **MOD-10** | **Chat & Broadcasts** | `Client/Client-Message.cpp` | `src/client/client_chat.rs` | `feat/mod-10-chat-broadcast` | ⚪ Available | - | Opcode 118 & system messages |

*Status Legend:*
- ⚪ **Available**: Open for anyone to claim.
- 🟡 **In Progress**: Currently being worked on in quarantine branch. Do NOT overwrite.
- 🟢 **Completed**: Implemented, verified against C++ & live server, merged into `main`.

---

## 🚀 How to Prompt an External AI / Remote Agent

Copy and paste this prompt to any external AI agent:

```text
Read CONTRIBUTING.md and README.md.

Task:
1. Check the Work Tracker in CONTRIBUTING.md.
2. Claim an available module (e.g. MOD-01).
3. Create and switch to a quarantine branch:
   git checkout main && git pull origin main
   git checkout -b feat/<module-id>-<short-description>
4. Update CONTRIBUTING.md to mark the module as "🟡 In Progress" with your details.
5. Implement the target file strictly following 1-1 OOP mapping against the referenced C++ source in GoonZu_Source/.
6. Verify with `cargo check`.
7. Commit and push ONLY to your quarantine branch:
   git push origin feat/<module-id>-<short-description>
   (DO NOT push to main).
```
