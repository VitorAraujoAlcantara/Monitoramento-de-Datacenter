<?php
include "./Sistema/ObjetoLeitor.php";

use Sistema\ObjetoLeitor;

$leitor = new ObjetoLeitor();
$leitor->lerAtual();
$dataAtual = new DateTime();
print $leitor->lampada.','.
        $leitor->luminosidade.','.
        $leitor->porta.','.
        $leitor->presenca.','.
        $leitor->temperatura.','.
        $leitor->lastData;
        
?>



