# Sistema de Monitoramento de Energia com ESP32 e Firebase / Energy Monitoring System with ESP32 and Firebase
# PEI – Arquitetura de Computadores / Computer Architecture Project

---

## 🇧🇷 Português

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

## 🇺🇸 English

## 1. Project Description

This project implements an electrical energy monitoring system using an ESP32 microcontroller, voltage and current sensors, and integration with the Firebase Realtime Database.

The system performs:

- RMS voltage measurement  
- RMS current measurement  
- Real power calculation  
- Periodic data upload to the cloud  
- Remote relay control via Firebase  

The application can be used for real-time electrical consumption monitoring and remote control of electrical loads.

---

## 2. Components Used

### Hardware

- ESP32  
- ZMPT101B voltage sensor  
- ACS712 current sensor  
- Relay module  
- WiFi network  

### Software / Libraries

- WiFi.h  
- Firebase_ESP_Client.h  
- EmonLib.h  
- Firebase Realtime Database
