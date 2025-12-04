// Importa o React e o hook useState e useEffect
// useState: permite criar e gerenciar estados no componente
// useEffect: permite executar código quando o componente é montado ou quando algo muda
import { useState, useEffect } from 'react';

// Importa as funções do Firebase Realtime Database
// ref: cria uma referência para um caminho específico no banco de dados
// onValue: escuta mudanças em tempo real em um caminho do banco
// off: remove os listeners (importante para evitar vazamentos de memória)
import { ref, onValue, off } from 'firebase/database';

// Importa a configuração do Firebase que criamos
import { database } from './firebaseConfig';

// Componente principal da aplicação
function App() {
  // Estados para armazenar os dados da tomada
  // useState retorna um array com [valor, funçãoParaAtualizar]
  
  // Estado para armazenar a voltagem (inicialmente 0)
  const [voltagem, setVoltagem] = useState(0);
  
  // Estado para armazenar o consumo em KWh (inicialmente 0)
  const [kwh, setKwh] = useState(0);
  
  // Estado para armazenar o status (true = ligado, false = desligado)
  const [status, setStatus] = useState(false);
  
  // Estado para controlar se está carregando os dados
  const [carregando, setCarregando] = useState(true);

  // useEffect: executa código quando o componente é montado na tela
  // O array vazio [] significa que só executa uma vez, quando o componente aparece
  useEffect(() => {
    // Cria uma referência para o caminho "tomada" no Firebase
    // database: nossa conexão com o Firebase
    // "tomada": o caminho onde estão os dados (root -> "tomada")
    const referenciaTomada = ref(database, 'tomada');

    // onValue: escuta mudanças em tempo real no caminho especificado
    // Quando qualquer dado em "tomada" mudar, esta função será executada
    const unsubscribe = onValue(referenciaTomada, (snapshot) => {
      // snapshot: contém os dados do Firebase no momento da mudança
      
      // Verifica se os dados existem
      if (snapshot.exists()) {
        // snapshot.val(): retorna um objeto JavaScript com todos os dados
        // Exemplo: { voltagem: 127, kwh: 5.5, status: true }
        const dados = snapshot.val();
        
        // Atualiza os estados com os valores do Firebase
        // Se o campo não existir, usa o valor padrão (0 ou false)
        setVoltagem(dados.voltagem || 0);
        setKwh(dados.kwh || 0);
        setStatus(dados.status || false);
        
        // Marca que os dados foram carregados
        setCarregando(false);
      } else {
        // Se não houver dados, mostra valores padrão
        console.log('Nenhum dado encontrado no caminho "tomada"');
        setCarregando(false);
      }
    }, (error) => {
      // Função de erro: executada se houver algum problema na conexão
      console.error('Erro ao ler dados do Firebase:', error);
      setCarregando(false);
    });

    // Cleanup: função executada quando o componente é desmontado
    // IMPORTANTE: sempre remova os listeners para evitar vazamentos de memória
    return () => {
      // off: remove o listener do Firebase
      // unsubscribe: a função retornada por onValue que remove o listener
      off(referenciaTomada, 'value', unsubscribe);
    };
  }, []); // Array vazio = executa apenas uma vez quando o componente monta

  // Se ainda estiver carregando, mostra uma mensagem
  if (carregando) {
    return (
      <div className="min-h-screen bg-gray-100 flex items-center justify-center">
        <div className="text-xl text-gray-600">Carregando dados...</div>
      </div>
    );
  }

  // Retorna o JSX (HTML do React) que será renderizado na tela
  return (
    // min-h-screen: altura mínima de 100% da tela
    // bg-gray-100: cor de fundo cinza claro
    // p-8: padding de 2rem (32px) em todos os lados
    <div className="min-h-screen bg-gray-100 p-8">
      {/* Container principal com largura máxima e centralizado */}
      <div className="max-w-6xl mx-auto">
        {/* Título do dashboard */}
        <h1 className="text-4xl font-bold text-gray-800 mb-8 text-center">
          Dashboard - Tomada Inteligente
        </h1>

        {/* Grid com 3 colunas para os cards (responsivo: 1 coluna no mobile, 3 no desktop) */}
        <div className="grid grid-cols-1 md:grid-cols-3 gap-6">
          
          {/* CARD 1: VOLTAGEM */}
          <div className="bg-white rounded-lg shadow-lg p-6">
            {/* Título do card */}
            <h2 className="text-xl font-semibold text-gray-700 mb-4">
              Voltagem
            </h2>
            
            {/* Valor da voltagem com destaque */}
            {/* text-4xl: texto muito grande */}
            {/* font-bold: texto em negrito */}
            {/* text-blue-600: cor azul */}
            <div className="text-4xl font-bold text-blue-600">
              {voltagem.toFixed(1)}V
              {/* toFixed(1): mostra apenas 1 casa decimal */}
            </div>
            
            {/* Descrição */}
            <p className="text-gray-500 mt-2">Tensão elétrica medida</p>
          </div>

          {/* CARD 2: CONSUMO (KWh) */}
          <div className="bg-white rounded-lg shadow-lg p-6">
            {/* Título do card */}
            <h2 className="text-xl font-semibold text-gray-700 mb-4">
              Consumo
            </h2>
            
            {/* Valor do consumo */}
            {/* text-green-600: cor verde */}
            <div className="text-4xl font-bold text-green-600">
              {kwh.toFixed(2)} KWh
              {/* toFixed(2): mostra 2 casas decimais */}
            </div>
            
            {/* Descrição */}
            <p className="text-gray-500 mt-2">Energia consumida</p>
          </div>

          {/* CARD 3: STATUS (Ligado/Desligado) */}
          <div className="bg-white rounded-lg shadow-lg p-6">
            {/* Título do card */}
            <h2 className="text-xl font-semibold text-gray-700 mb-4">
              Status
            </h2>
            
            {/* Valor do status com cor condicional */}
            {/* Se status for true (ligado), mostra verde; se false (desligado), mostra vermelho */}
            <div className={`text-4xl font-bold ${status ? 'text-green-600' : 'text-red-600'}`}>
              {/* Operador ternário: condição ? valorSeVerdadeiro : valorSeFalso */}
              {status ? 'LIGADO' : 'DESLIGADO'}
            </div>
            
            {/* Indicador visual adicional (bolinha colorida) */}
            <div className="flex items-center mt-4">
              {/* Bolinha que muda de cor baseado no status */}
              <div className={`w-4 h-4 rounded-full mr-2 ${status ? 'bg-green-500' : 'bg-red-500'}`}></div>
              
              {/* Texto descritivo */}
              <p className={`text-sm font-medium ${status ? 'text-green-600' : 'text-red-600'}`}>
                {status ? 'Tomada está ligada' : 'Tomada está desligada'}
              </p>
            </div>
          </div>

        </div>

        {/* Informação adicional no rodapé */}
        <div className="mt-8 text-center text-gray-500 text-sm">
          Dados atualizados em tempo real do Firebase
        </div>
      </div>
    </div>
  );
}

// Exporta o componente para ser usado em outros arquivos
export default App;

