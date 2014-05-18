<?php
include "./Sistema/ObjetoLeitor.php";

use Sistema\ObjetoLeitor;

$leitor = new ObjetoLeitor();
$leitor->lerAtual();

$par = $_GET['par'];

$data = $leitor->lerIntervalo($par, 0);
$sep = '';
$tabela = array();
if ($data) {
    while ($obj = $data->fetch_object()) {
        array_push($tabela, array(
            'arg'  => $obj->leit_data,
            'val'  => $obj->leit_valor /($par=='t'?10:1) )
                );
        //$sep = ',';
    }
}
echo( json_encode($tabela) );

