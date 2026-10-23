# Audit & Re-Verification Plan (MOD-01 to MOD-09)

> **Document Status:** Active Audit & Correction Directive  
> **Target Repository:** `rust_Iwcgz_client`  
> **Author:** Lead QC (PsiGinger & Aun)  

---

## 🎯 Executive Summary
Following an end-to-end architectural review (/scrutinize) on **MOD-09**, it was discovered that while offline mock unit tests passed in under 30 seconds, the code suffered from severe structural flaws:
1. **Blocking Network Polling:** `poll_game_packet()` issues a blocking `recv_frame().await`, freezing the event loop and starving the 10-second heartbeat ticker.
2. **Protocol Mismatch:** Opcode 215 was falsely mapped to `ResponsePersonIp` instead of the vital status push.
3. **Stubbed Logic:** Critical state synchronization branches were left as empty stubs (`// not parsed in detail`).

To ensure total system integrity and prevent false positives from propagating, every module from **MOD-01 to MOD-09** must be audited against real socket semantics and live server wire truth.

---

## 📋 Comprehensive Audit Matrix (MOD-01 to MOD-09)

| Module ID | Module Name | Current QC Status | Audit Finding / Scrutiny Focus | Required Action |
|---|---|---|---|---|
| **MOD-01** | **Cargo & Project Scaffold** | 🟢 Passed | Baseline configs, flags, and local crate dependencies are verified. | No action required. Stable foundation. |
| **MOD-02** | **State Box (FSM)** | 🟢 Passed | Atomic transitions and enum values mirror `StateBox.h` 1-to-1. | No action required. Stable. |
| **MOD-03** | **Network Session** | 🟡 Needs Enhancement | `NetSession` currently holds a monolithic `TcpStream`. Does not support concurrent non-blocking reading and writing. | Needs split stream (`into_split()` or `tokio::select!` safe read) to avoid blocking loops. |
| **MOD-04** | **World List** | 🟢 Passed | Wire packing matches exact 428-byte C-struct layout. | Verified. Stable. |
| **MOD-05** | **Char Client Data** | 🟢 Passed | `MakeCharInfo` unpacking mirrors `MAKE_CHAR_INFO_PAYLOAD_LEN` (592 bytes). | Verified. Stable. |
| **MOD-06** | **CltClient Core Class** | 🟢 Passed | Structural aggregation of managers mirrors `Client.h`. | Ready to host verified handlers. |
| **MOD-07** | **Front Login Handler** | 🟡 Needs Live Verification | Code path constructs Opcode 21 and 1002 frames correctly in theory, but has not performed a live TCP handshake. | Run against local Front (`127.0.0.1:5700`) or Ngrok. |
| **MOD-08** | **Game Entry Handler** | 🟡 Needs Live Verification | 15-slot char list loop is present. Session key link handshake implemented. | Requires live validation against Game server. |
| **MOD-09** | **In-Game Event Loop** | 🔴 REJECTED | **Blocker:** `recv_frame().await` blocks heartbeat execution. Fake opcode mapping. | **Send back for complete rework.** |

---

## 🛠️ Correction Directive for MOD-09 (Prompt for AI Agent)

The AI contributor responsible for MOD-09 must resolve the following blocking issues before resubmitting:

1. **Non-blocking Event Loop:**
   - In `run_world_loop`, do NOT call a blocking `poll_game_packet()` that waits indefinitely.
   - Use `tokio::select!` properly so that if no packet arrives on the socket, the 10-second ticker still ticks and fires `send_heartbeat()`.
2. **Correct Opcode Definitions:**
   - Map Opcode 215 properly (HP/MP status push) and parse delta vitals into `self.my_char`.
   - Map Opcode 200 properly (Character position/state update) and parse coordinates into `self.my_char`.
3. **Validation Test Requirement:**
   - Write an asynchronous integration unit test using `tokio::time::pause()` or simulated duplex stream proving that heartbeat fires repeatedly even when no inbound packets are received.
