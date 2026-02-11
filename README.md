# PEI-arquitetura-de-computadores
## Sistema de Monitoramento de Energia com ESP32 e Firebase

## 1. Descrição do Projeto

Este projeto implementa um sistema de medição de energia elétrica utilizando um ESP32, sensores de tensão e corrente, e integração com o Firebase Realtime Database.  

O sistema realiza:

- Medição de tensão RMS
- Medição de corrente RMS
- Cálculo de potência real
- Envio periódico dos dados para a nuvem
- Controle remoto de um relay via Firebase

A aplicação pode ser utilizada para monitoramento de consumo elétrico em tempo real e controle remoto de cargas elétricas.

---

## 2. Componentes Utilizados

### Hardware

- ESP32
- Sensor de Tensão ZMPT101B
- Sensor de Corrente ACS712
- Módulo Relay
- Rede WiFi

### Software / Bibliotecas

- WiFi.h
- Firebase_ESP_Client.h
- EmonLib.h
- Firebase Realtime Database

---
