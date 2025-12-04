// Importa as funções necessárias do Firebase SDK v9
// getApp: obtém uma instância do app Firebase (ou cria uma nova)
// initializeApp: inicializa o Firebase com as configurações
// getDatabase: obtém uma referência ao Realtime Database
import { getApp, initializeApp } from 'firebase/app';
import { getDatabase } from 'firebase/database';

// Configurações do seu projeto Firebase
// IMPORTANTE: Substitua estes valores pelas suas credenciais reais do Firebase
// Você encontra essas informações no Console do Firebase (https://console.firebase.google.com)
// Vá em: Configurações do Projeto > Seus apps > Configuração
const firebaseConfig = {
  // API Key: Chave de API do seu projeto Firebase
  // Você encontra isso no Console do Firebase > Configurações do Projeto
  apiKey: "AIzaSyC8bqCXthUftcj2c0dzHIW3j_gReOd4rPE",
  
  // Auth Domain: Domínio de autenticação (geralmente: seu-projeto.firebaseapp.com)
  // Se não usar autenticação, pode deixar vazio ou usar o padrão
  authDomain: "arquitetura-de-computado-54e7e.firebaseapp.com",
  
  // Database URL: URL do seu Realtime Database
  // IMPORTANTE: Esta é a URL completa do seu Realtime Database
  // Formato: https://seu-projeto-default-rtdb.firebaseio.com
  // Você encontra isso no Console do Firebase > Realtime Database > Dados
  databaseURL: "https://arquitetura-de-computado-54e7e-default-rtdb.firebaseio.com",
  
  // Project ID: ID do seu projeto no Firebase
  projectId: "arquitetura-de-computado-54e7e",
  
  // Storage Bucket: Bucket de armazenamento (opcional, mas necessário para Storage)
  // Se não usar Storage, pode deixar vazio
  storageBucket: "arquitetura-de-computado-54e7e.appspot.com",
  
  // Messaging Sender ID: ID do remetente de mensagens (para Cloud Messaging)
  // Se não usar Cloud Messaging, pode deixar vazio
  messagingSenderId: "",
  
  // App ID: ID único do seu app
  // Você encontra isso no Console do Firebase > Configurações do Projeto > Seus apps
  appId: ""
};

// Função para inicializar o Firebase
// Esta função verifica se já existe uma instância do app Firebase
// Se existir, retorna ela; se não, cria uma nova
function initFirebase() {
  try {
    // Tenta obter uma instância existente do app
    return getApp();
  } catch (error) {
    // Se não existir, inicializa uma nova instância com as configurações
    return initializeApp(firebaseConfig);
  }
}

// Inicializa o Firebase e armazena a instância do app
const app = initFirebase();

// Obtém uma referência ao Realtime Database
// Esta referência será usada para ler e escrever dados no Firebase
const database = getDatabase(app);

// Exporta a referência do database para ser usada em outros arquivos
// Quando você importar este arquivo em outro componente, poderá usar 'database'
export { database };

// Exporta também o app caso você precise dele em outros lugares
export default app;

