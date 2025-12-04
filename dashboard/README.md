# Dashboard - Tomada Inteligente

Dashboard simples para visualizar dados em tempo real da sua tomada inteligente conectada ao Firebase Realtime Database.

## 🚀 Tecnologias

- **React** (com Vite)
- **Tailwind CSS** (para estilização)
- **Firebase Web SDK v9** (para conexão com o Realtime Database)

## 📋 Pré-requisitos

- Node.js instalado (versão 16 ou superior)
- npm ou yarn instalado
- Projeto Firebase configurado com Realtime Database

## 🔧 Instalação

1. Navegue até a pasta do projeto:
```bash
cd dashboard
```

2. Instale as dependências:
```bash
npm install
```

## ⚙️ Configuração do Firebase

1. Abra o arquivo `src/firebaseConfig.js`

2. Substitua as configurações com os dados do seu projeto Firebase:
   - Acesse o [Console do Firebase](https://console.firebase.google.com)
   - Vá em **Configurações do Projeto** (ícone de engrenagem)
   - Role até **Seus apps** e clique em **Configuração**
   - Copie os valores e cole no arquivo `firebaseConfig.js`

3. Certifique-se de que a estrutura dos dados no Firebase está assim:
```
root
  └── tomada
      ├── voltagem: 127
      ├── kwh: 0
      └── status: true
```

## 🎯 Como Executar

1. Inicie o servidor de desenvolvimento:
```bash
npm run dev
```

2. Abra o navegador em `http://localhost:5173`

3. O dashboard irá conectar automaticamente ao Firebase e mostrar os dados em tempo real!

## 📁 Estrutura do Projeto

```
dashboard/
├── src/
│   ├── App.jsx              # Componente principal com a lógica do dashboard
│   ├── firebaseConfig.js    # Configuração de conexão com o Firebase
│   ├── main.jsx             # Ponto de entrada da aplicação
│   └── index.css            # Estilos globais com Tailwind
├── index.html               # HTML principal
├── package.json             # Dependências do projeto
├── vite.config.js           # Configuração do Vite
├── tailwind.config.js       # Configuração do Tailwind CSS
└── postcss.config.js        # Configuração do PostCSS
```

## 📝 Funcionalidades

- ✅ Conexão automática com Firebase Realtime Database
- ✅ Leitura de dados em tempo real (voltagem, KWh, status)
- ✅ Interface visual com cards coloridos
- ✅ Indicador visual de status (verde = ligado, vermelho = desligado)
- ✅ Atualização automática quando os dados mudam no Firebase

## 🎨 Personalização

Você pode personalizar as cores e estilos editando as classes do Tailwind no arquivo `src/App.jsx`.

## 📚 Aprendizado

Este projeto foi criado com comentários extensivos em português para facilitar o aprendizado. Leia os arquivos `App.jsx` e `firebaseConfig.js` para entender como tudo funciona!

