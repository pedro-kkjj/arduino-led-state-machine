 
int led2 = 4; //vermelho
int led1 = 5; //verde
int botao = 6;
int fase = 0;
int modo = 0;
int botAnterior = 0; //estado botão
int botAtual = 0;

int led1Ativo = 0;
int led2Ativo = 0;            

unsigned long tempoLed1 = 0;
unsigned long tempoLed2 = 0;

bool estadoLed1 = LOW;
bool estadoLed2 = LOW;

unsigned long tempoInicioClique = 0;
unsigned long tempoFimClique = 0;
unsigned long duracaoClique = 0;


void desligarLed(int led)
{ 
  digitalWrite(led, LOW);
}

// Liga o led se ele estiver ativo. (desliga se não)
void ligarSeAtivo(int led, int ativo)
{
  if (ativo == 1)
  {
    digitalWrite(led, HIGH);
  }else{
    digitalWrite(led, LOW);
  }
}

// Verifica se o led está ativo e, se estiver, pisca em certo intervalo. (desliga se não)
void piscarSeAtivo(int led, int ativo, unsigned long &tempo, bool &estado, int intervalo)
{
 if (ativo == 1)
 { 
   if (millis() - tempo >= intervalo)
   {
     tempo = millis();
     estado = !estado;
     digitalWrite(led, estado);
   }
 }
 else
 { 
   desligarLed(led);
 } 
}

// Define quais leds participam da fase atual.
void definirLedsAtivos()
{
 if(fase == 1)
 {
   led1Ativo = 1;
   led2Ativo = 0;      
 }
 else if (fase == 2)
 {
   led1Ativo = 0;
   led2Ativo = 1;
 }
 else if (fase == 3)
 {
   led1Ativo = 1;
   led2Ativo = 1;
 }
 else
 {
   led1Ativo = 0;
   led2Ativo = 0;   
   desligarLed(led1);
   desligarLed(led2); 
 }
}

void detectarBotao()
{
 if (botAnterior == LOW && botAtual == HIGH)
 {
  // Inicia a medição da duração do clique e reinicia o estado dos LEDs.
  tempoInicioClique = millis();    
  tempoLed1 = millis(); 
  tempoLed2 = millis();  

  estadoLed1 = LOW;
  estadoLed2 = LOW; 
   
 }
 if (botAnterior == HIGH && botAtual == LOW) 
 {  
 // Ao soltar o botão, calcula a duração do clique para diferenciar.
 // clique curto (troca a fase) de clique longo (troca o modo).

  tempoFimClique = millis();  
  duracaoClique = tempoFimClique - tempoInicioClique;   
    
  if (duracaoClique < 300)
  {    
   fase++;     
   if (fase > 3 )fase = 0;   
    
   Serial.print("-CLIQUE CURTO- ");        
  }
  else if (duracaoClique > 300)
  {      
   modo++;
   if (modo > 3 )modo = 0;  
    
   Serial.print("-CLIQUE LONGO- ");  
  } 
 }
  botAnterior = botAtual;
}

// Exibe no monitor serial a fase e o modo atuais.
void mostrarInfo()
{ 
 Serial.print("Fase: ");
 Serial.print(fase);
 Serial.print(". Modo: ");
 Serial.println(modo);
}

// Executa o comportamento dos LEDs de acordo com o modo selecionado.  
void executarModo()
{
 if (modo == 1)
 { 
  piscarSeAtivo(led1, led1Ativo, tempoLed1, estadoLed1, 500);
  piscarSeAtivo(led2, led2Ativo, tempoLed2, estadoLed2, 500);
 }
 else if (modo == 2)
 {
  piscarSeAtivo(led1, led1Ativo, tempoLed1, estadoLed1, 200);
  piscarSeAtivo(led2, led2Ativo, tempoLed2, estadoLed2, 200);
 }
 else if (modo == 3)  
 {
  ligarSeAtivo(led1, led1Ativo);
  ligarSeAtivo(led2, led2Ativo);  
 }
 else
 {
  desligarLed(led1);
  desligarLed(led2);
 } 
}

void setup()
{
 pinMode(led1, OUTPUT);
 pinMode(led2, OUTPUT);
 pinMode(botao, INPUT);
 Serial.begin(9600);
}


void loop()
{
 botAtual = digitalRead(botao);
  
 mostrarInfo();
 detectarBotao();
 definirLedsAtivos();
 executarModo(); 

}
