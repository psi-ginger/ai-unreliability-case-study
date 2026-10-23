# RCA-2026-10-23-MOD-09-BLOCKING-LOOP

## 1. Executive Summary
MOD-09's in-game loop starved the heartbeat ticker because poll_game_packet blocked on recv_frame().await indefinitely during silent socket periods. Opcode 215 was incorrectly mapped to ResponsePersonIp and stubbed.

## 2. Root Cause
recv_frame().await inside tokio::select! never completed without inbound data, so the 10-second ticker branch could not fire. Additionally opcode 215 (HP/MP) was mislabeled and not parsed.

## 3. Evidence
Before: poll_game_packet called session.recv_frame().await directly. After: uses tokio::time::timeout(100ms). Routing now parses 215 -> update_vitals, 200 -> update_pos.

## 4. Resolution
Implemented timeout-bounded poll_game_packet and proper opcode routing. Integrated main.rs end-to-end lifecycle.

## 5. Prevention
Unit test heartbeat frame serialization and timeout-bounded polling ensures ticker fires regardless of inbound traffic.
