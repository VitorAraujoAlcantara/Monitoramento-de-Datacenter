<?php
include "./Sistema/ObjetoLeitor.php";

use Sistema\ObjetoLeitor;

$leitor = new ObjetoLeitor();
$leitor->lerAtual();

print $leitor->lampada.','.
        $leitor->luminosidade.','.
        $leitor->porta.','.
        $leitor->presenca.','.
        $leitor->temperatura.',';
?>



