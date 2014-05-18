<?php
namespace Sistema;

include 'conexao.php';

use Sistema\conexao;
class ObjetoLeitor{
    public $temperatura;
    public $luminosidade;
    public $porta;
    public $presenca;
    public $lampada;
    
    public function lerAtual(){
        $this->lampada      = $this->lerValorAtual('s');
        $this->temperatura  = $this->lerValorAtual('t');
        $this->luminosidade = $this->lerValorAtual('l');
        $this->porta        = $this->lerValorAtual('q');
        $this->presenca     = $this->lerValorAtual('r');
        
        $this->lampada      = $this->lampada == 1 ? 'Ligada': 'Desligada';
        $this->porta        = $this->porta == 0 ? 'Aberta': 'Fechada';
        $this->presenca     = $this->presenca == 1? 'Detectado': 'Não Detectado';
        $this->luminosidade = (int) explode(' ',str_replace("\n", '', $this->luminosidade))[0];
        $this->temperatura  = (float) explode(' ',str_replace("\n", '', $this->temperatura))[0]/10;
        
        
    }
    
    protected function criarArquivo($chamado){
        $fp = fopen($chamado,'w+'); 
        fclose($fp);
    }
    
    protected function lerValorAtual($tipo){
        
        /* @var $query \mysqli_result */
        $query = conexao::executaConsulta("SELECT * FROM leituras WHERE leituras.leit_tipo = '".$tipo."' ORDER BY leituras.leit_id DESC LIMIT 1 ");
        $row = $query->fetch_row();
        
        return $row[2];
    }
    
    public function lerIntervalo($tipo,$parametro){
        $tempo = 0;
        switch ($parametro){
            case 01: $tempo = 10;
                break;
            case 02: $tempo = 30;
                break;
            case 03: $tempo = 360;
                break;
            case 04: $tempo = 1440;
                break;            
        }
         if ( $tempo > 30 )
        {
                $select = "SELECT DISTINCT DATE_FORMAT( `leit_data` , '%d/%c/%Y %H' ) AS `leit_data` , AVG(leit_valor) AS `leit_valor`, leit_tipo  FROM leituras WHERE leituras.leit_tipo = '". $tipo."' AND leituras.leit_data >=  current_timestamp - INTERVAL ".$tempo." MINUTE GROUP BY  DATE_FORMAT( `leit_data` , '%d/%c/%Y %H' ) ORDER BY leituras.leit_data ";
        }
        else
        if ( $tempo > 10 ) {
		$select = "SELECT DISTINCT DATE_FORMAT( `leit_data` , '%d/%c/%Y %H:%i' ) AS `leit_data` , AVG(leit_valor) AS `leit_valor`, leit_tipo  FROM leituras WHERE leituras.leit_tipo = '". $tipo."' AND leituras.leit_data >=  current_timestamp - INTERVAL ".$tempo." MINUTE GROUP BY DATE_FORMAT( `leit_data` , '%d/%c/%Y %H:%i' ) ORDER BY  leituras.leit_data ";
         }
        else
        if ( $tempo <= 10 )
        {
            $select = "SELECT DATE_FORMAT( `leit_data` , '%d/%c/%Y %H:%i:%ss' ) AS `leit_data`, leit_valor, leit_tipo FROM leituras WHERE leituras.leit_tipo = '".$tipo."' AND leituras.leit_data >=  current_timestamp - INTERVAL ".$tempo." MINUTE ORDER BY leituras.leit_id ";           
        }
        
        /* @var $query \mysqli_result */
        $query = conexao::executaConsulta($select);       
        return $query;
            
    }
        
    
    protected function lerValor($chamado ){   
        $this->criarArquivo($chamado);        
        if ( !file_exists("RET_".$chamado)){
            return;
        }
        $fp = fopen('RET_'.$chamado, "r"); 
        $retorno = fgets($fp,4096); 
        fclose($fp);   
        return $retorno;
    }
}
