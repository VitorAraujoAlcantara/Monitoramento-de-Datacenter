<?php
namespace Sistema;
use mysqli;       
class conexao {

    public static function getConexao() {
        try {
            $host = "192.168.0.100";
            $port = 3306;
            $socket = "";
            $user = "root";
            $password = "123Mudar";
            $dbname = "monitora_servidor";

            $con = new mysqli($host, $user, $password, $dbname, $port, $socket)
                    or die('Could not connect to the database server' . mysqli_connect_error());

            //$con->close();
            return $con;
        } catch (Exception $e) {
            print "Error!: " . $e->getMessage() . "<br/>";
            die();
        }
    }

    public static function executaConsulta($consulta) {
        try {
            $con = self::getConexao();
            $query = $con->query($consulta);
            $con->close();            
            return $query;
        } catch (Exception $exc) {
            echo $exc->getTraceAsString();
        }
    }

}
