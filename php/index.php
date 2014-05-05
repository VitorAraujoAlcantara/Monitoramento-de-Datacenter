<?php

  $fp = fopen('LUS','w+'); fclose($fp);
   $fp = fopen('TEMPERATURA','w+'); fclose($fp);
   $fp = fopen('LUMINOSIDADE','w+'); fclose($fp);
   $fp = fopen('PORTA','w+'); fclose($fp);
   $fp = fopen('PRESENCA','w+'); fclose($fp);   

   $fp = fopen("RET_LUS", "r"); $lampada = fgets($fp,4096); fclose($fp);
   $fp = fopen("RET_TEMPERATURA", "r"); $temperatura = fgets($fp,4096); fclose($fp);   
   $fp = fopen("RET_PORTA", "r"); $porta = fgets($fp,4096); fclose($fp);
   $fp = fopen("RET_LUMINOSIDADE", "r"); $luminosidade = fgets($fp,4096); fclose($fp);         
   $fp = fopen("RET_PRESENCA", "r"); $presenca = fgets($fp,4096); fclose($fp); 
   if ( isset($_POST['liga_ar'] ) ) {
      $fp = fopen('LIGA_AR','w+'); fclose($fp);
   }
   
   if ( isset($_POST['desliga_lampada']) ) {
      $fp = fopen('DESLIGA_LUS','w+'); fclose($fp);   
   }
   
   if ( isset($_POST['liga_lampada']) ) {
      $fp = fopen('LIGA_LUS','w+'); fclose($fp);   
   }   

   $luminosidade = (int) explode(' ',str_replace("\n", '', $luminosidade))[0];
   $temperatura = (float) explode(' ',str_replace("\n", '', $temperatura))[0]/10;
   
   if (isset($_POST['chk_temperatura'])) {
      $chk_temperatura =true;
   }
   
   if (isset($_POST['chk_atualiza'])) {
      $chk_atualiza =true;
   }
   if (isset($_POST['chk_luminosidade'])) {
      $chk_luminosidade =true;
   }   
   if (isset($_POST['chk_porta'])) {
      $chk_porta =true;
   }   
   if (isset($_POST['chk_lampada'])) {
      $chk_lampada =true;
   }   
   if (isset($_POST['chk_presenca'])) {
      $chk_presenca = true;
   }
   
   if ( isset ($_POST['data'])) 
   {
      $date = new DateTime($_POST['data']);
   }
   else
   {
         $date = new DateTime(null);
   }
   
   if ( isset ($_POST['hora1']) )
   {
      $hora1 = $_POST['hora1'];      
   }
   
   if ( isset ($_POST['hora2']) )
   {
      $hora2 = $_POST['hora2'];
   }
   
   if ( $hora1 == '' )
      unset($hora1);
      
   if ( $hora2 == '' )
      unset($hora2);
   
   
   
   $filename = $date->format ( 'Ymd')  . '_retorno.txt';
      $data1 ='';
      $modulo = '';
      $delimitador = '';
      $hora = '';
      if ( file_exists($filename) ) {
   
   $fp = fopen($filename, "r"); 
   


      while (!feof ($fp)) {
           //LÊ UMA
           $linha = fgets($fp,4096);
           $lst = explode(' ', $linha );
           $modulo = '';

            if ( count($lst) < 2 ) continue;
            if ( ! isset($lst[2])) continue;
            
            if (isset($hora1)) {
               if ( $lst[0] < $hora1 ) 
                  continue;
            }
            
            if (isset($hora2)) {
               if ($lst[0] > $hora2 ) 
                  continue;
            }
            
           $lst[2] = trim($lst[2]);


           switch ( $lst[2] )
           {
               case 'c': 
                  if ( isset($chk_temperatura) )
                  $modulo = 'Temperatura';
                  break;
               case 'l':
                  if ( isset($chk_luminosidade) )
                  $modulo = 'Luminosidade';
                  break;
               case 's':
                  if ( isset($chk_porta) )
                  $modulo = 'Porta';
                  break;
               case 'p':
                  if ( isset($chk_presenca) )            
                  $modulo = 'Presenca';
                  break;
               case 'x':
                  if ( isset($chk_lampada) )            
                  $modulo = 'Lampada';
                  break;
               default: 
                  $modulo = '';
                  break;
                  
           }
           if ( $hora != $lst[0] )
           {
            if ( $delimitador == ',' ){
               $data1 = $data1 . '} ';
            }
            $data1 = $data1 . $delimitador. "{ hora: '". $lst[0]."'";
            $hora = $lst[0];
           }
           $value = $lst[1];
           if ( $modulo == 'Temperatura' ) {
            $value = (float) $value / 10 ;
           }
           if ($modulo ) {
            $data1 = $data1 . ", ".$modulo . ": ". $value."";
           }
           $delimitador = ',';
           
           
      }//F   
      $data1 = $data1 . "} ";
      fclose($fp); 
   }
 
   $delimitador = '';

   
?>
<html>
   <head>
      <meta charset="utf-8" />
      <script type="text/javascript" src="js/jquery-2.0.3.min.js"></script>
      <!--script type="text/javascript" src="knockout-3.0.0.js"></script-->
      <!--script type="text/javascript" src="angular.min.js"></script-->
      <script type="text/javascript" src="js/globalize.min.js"></script>
      <script type="text/javascript" src="js/dx.chartjs.js"></script>


      
      <script type="text/javascript">
         var chartDataSource = [
            
            <?= $data1 ?>
         ];
         $(function () {             
             $("#chartContainer").dxChart({
                commonSeriesSettings: {
                    type: 'fullStackedArea'
                },
                tooltip: {
                    enabled: true,
                    percentPrecision: 2                    
                },
                
                dataSource: chartDataSource,
                commonSeriesSettings: {
                    argumentField: 'hora'
                }
                ,series: [
<?php                if ( isset($chk_temperatura) ) { ?>               
                <?php echo $delimitador; $delimitador = ',';?>
                {
                    name: 'Temperatura',            
                    valueField: 'Temperatura'
                } 
<?php } ?>
<?php                if ( isset($chk_luminosidade) ) { ?>                                
                <?php echo $delimitador; $delimitador = ',';?> {
                    name: 'Luminosidade',
                    valueField: 'Luminosidade'
                }
<?php } ?>                
<?php                if ( isset($chk_porta) ) { ?>                         
                <?php echo $delimitador; $delimitador = ',';?>{
                    name: 'Porta',
                    valueField: 'Porta'
                }
<?php } ?>
<?php                if ( isset($chk_presenca) ) { ?>                               
                <?php echo $delimitador; $delimitador = ',';?>{
                    name: 'Presença',
                    valueField: 'Presenca'
                }
<?php  } ?>
<?php                if ( isset($chk_lampada) ) { ?>               
                <?php echo $delimitador; $delimitador = ',';?>{ 
                    name: 'Lâmpada',
                    valueField: 'Lampada'
                }
<?php } ?>                
                ]
            });
         });        
      </script>
   </head>
   <body>
      <h1>Monitoramento do DataCenter </h1>
      <form action='index.php' method='POST' id="form1">
         <div>
            Presença: <?= strpos($presenca,'1')>0?'Ativo':'Inativo' ?> 
         </div>
         <div>
            Porta: <?=  strpos($porta,'1')> 0?'Ativo':'Inativo'  ?>
         </div>
         <div>
            Luminosidade: <?= $luminosidade ?> 
         </div>
         <div>
            Lâmpada: <?= strpos($lampada,'1') > 0 ? 'Ligada':'Desligada' ?>
         </div>
         <div>
            Temperatura: <?= $temperatura ?>
         </div>           
         <br>
         <input type='submit' name='liga_ar' value='Ligar ar condicionado' />       
         <input type='submit' name='liga_lampada' value='Ligar lâmpada' />
         <input type='submit' name='desliga_lampada' value='Desligar lâmpada' />             
         <input type='submit' name='atualizar' value='Atualizar' />
         <br>
         <input type='date' name='data' id='data' <?php echo isset ($_POST['data']) ? 'value='.$_POST['data']:'' ?> />
         <input type='time' name='hora1' id='hora1' <?php echo isset ($_POST['hora1']) ? 'value='.$_POST['hora1']:'00:00' ?> />
         <input type='time' name='hora2' id='hora2' <?php echo isset ($_POST['hora2']) ? 'value='.$_POST['hora2']:'00:00' ?> />
         <br>
         <label>Temperatura<input type='checkbox' name='chk_temperatura' <?php echo  isset($chk_temperatura) ? 'checked=true':'' ?>     /></label><br>
         <label>Luminosidade<input type='checkbox' name='chk_luminosidade' <?php echo  isset($chk_luminosidade) ? 'checked=true':'' ?> /></label><br>
         <label>Porta<input type='checkbox' name='chk_porta' <?php echo  isset($chk_porta) ? 'checked=true':'' ?> /></label><br>
         <label>Presença<input type='checkbox' name='chk_presenca' <?php echo  isset($chk_presenca) ? 'checked=true':'' ?> /></label><br>
         <label>Lâmpada<input type='checkbox' name='chk_lampada' <?php echo  isset($chk_lampada) ? 'checked=true':'' ?> /></label><br>
         <!--<label>Auto-Atualiza<input type='checkbox' name='chk_atualiza' <?php echo  isset($chk_atualiza) ? 'checked=true':'' ?> /></label><br>-->
         
      </form>
      
      <div id="chartContainer" style="max-width:100%;min-height: 300px;"></div>

      
   </body>
</html>
