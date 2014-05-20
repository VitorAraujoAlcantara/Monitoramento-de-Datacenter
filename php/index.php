<?php
include "./Sistema/ObjetoLeitor.php";

use Sistema\ObjetoLeitor;

$leitor = new \Sistema\ObjetoLeitor();
$leitor->lerAtual();
$par = 01;
if (isset($_GET['par'])) {
    $par = $_GET['par'];
}
// TEMPERATURA
$data = $leitor->lerIntervalo('t', $par);
$tabela_temperatura = '';
$sep = '';
if ($data) {
    while ($obj = $data->fetch_object()) {
        $tabela_temperatura = $tabela_temperatura . $sep . '{' .
                'arg: "' . $obj->leit_data . '",' .
                'val: ' . ($obj->leit_valor / 10.0) .
                '}';
        $sep = ',';
    }
}
$tabela_temperatura = $tabela_temperatura == '' ? null : $tabela_temperatura;

// LUMINOSIDADE
$data = $leitor->lerIntervalo('l', $par);
$tabela_luminosidade = '';
$sep = '';
if ($data) {
    while ($obj = $data->fetch_object()) {
        $tabela_luminosidade = $tabela_luminosidade . $sep . '{' .
                'arg: "' . $obj->leit_data . '",' .
                'val: ' . $obj->leit_valor .
                '}';
        $sep = ',';
    }
}
$tabela_luminosidade = $tabela_luminosidade == '' ? null : $tabela_luminosidade;

// PORTA
$data = $leitor->lerIntervalo('q', $par);
$tabela_porta = '';
$sep = '';
if ($data) {
    while ($obj = $data->fetch_object()) {
        $tabela_porta = $tabela_porta . $sep . '{' .
                'arg: "' . $obj->leit_data . '",' .
                'val: ' . $obj->leit_valor .
                '}';
        $sep = ',';
    }
}
$tabela_porta = $tabela_porta == '' ? null : $tabela_porta;

// PRESENCA
$data = $leitor->lerIntervalo('r', $par);
$tabela_presenca = '';
$sep = '';
if ($data) {
    while ($obj = $data->fetch_object()) {
        $tabela_presenca = $tabela_presenca . $sep . '{' .
                'arg: "' . $obj->leit_data . '",' .
                'val: ' . $obj->leit_valor .
                '}';
        $sep = ',';
    }
}
$tabela_presenca = $tabela_presenca == '' ? null : $tabela_presenca;

// lampada
$data = $leitor->lerIntervalo('s', $par);
$tabela_lampada = '';
$sep = '';
if ($data) {
    while ($obj = $data->fetch_object()) {
        $tabela_lampada = $tabela_lampada . $sep . '{' .
                'arg: "' . $obj->leit_data . '",' .
                'val: ' . $obj->leit_valor .
                '}';
        $sep = ',';
    }
}
$tabela_lampada = $tabela_lampada == '' ? null : $tabela_lampada;
?>
<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <meta name="description" content="">
        <meta name="author" content="">
        <link rel="shortcut icon" href="../../assets/ico/favicon.ico">

        <title>Monitoramento do DataCenter - V 1.0 </title>

        <!-- Bootstrap core CSS -->
        <link href="css/bootstrap.min.css" rel="stylesheet">

        <!-- Custom styles for this template -->
        <link href="css/dashboard.css" rel="stylesheet">

        <!-- Just for debugging purposes. Don't actually copy this line! -->
        <!--[if lt IE 9]><script src="../../assets/js/ie8-responsive-file-warning.js"></script><![endif]-->

        <!-- HTML5 shim and Respond.js IE8 support of HTML5 elements and media queries -->
        <!--[if lt IE 9]>
          <script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv.js"></script>
          <script src="https://oss.maxcdn.com/libs/respond.js/1.4.2/respond.min.js"></script>
        <![endif]-->        
    </head>

    <body>

        <div class="navbar navbar-inverse navbar-fixed-top" role="navigation">
            <div class="container-fluid">
                <div class="navbar-header">
                    <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
                        <span class="sr-only">Toggle navigation</span>
                        <span class="icon-bar"></span>
                        <span class="icon-bar"></span>
                        <span class="icon-bar"></span>
                    </button>
                    <a class="navbar-brand" href="#">Monitora Servidor</a>
                </div>
                <div class="navbar-collapse collapse">
                    <ul class="nav navbar-nav navbar-right">
                        <li <?= $par == '00' ? 'class="active"' : '' ?>><a href="index.php?par=00">Atual</a></li>
                        <li <?= $par == '01' ? 'class="active"' : '' ?>><a href="index.php?par=01">Ultimos 10 minutos</a></li>
                        <li <?= $par == '02' ? 'class="active"' : '' ?>><a href="index.php?par=02">Ultimos 30 minutos</a></li>
                        <li <?= $par == '03' ? 'class="active"' : '' ?>><a href="index.php?par=03">Ultimas 6 horas</a></li>
                        <li <?= $par == '04' ? 'class="active"' : '' ?>><a href="index.php?par=04">Ultimas 24 horas</a></li>
                        <li <?= $par == '05' ? 'class="active"' : '' ?>><a href="index.php?par=05">Customizado</a></li>
                    </ul>		    
                </div>
            </div>
        </div>

        <div class="container-fluid">
            <div class="row">
                <div class="col-sm-3 col-md-2 sidebar">
                    <ul class="nav nav-sidebar">
                        <li <?= $par == '00' ? 'class="active"' : '' ?> ><a href="index.php?par=00">Atual</a></li>
                        <li <?= $par == '01' ? 'class="active"' : '' ?>><a href="index.php?par=01">Ultimos 10 minutos</a></li>
                        <li <?= $par == '02' ? 'class="active"' : '' ?>><a href="index.php?par=02">Ultimos 30 minutos</a></li>
                        <li <?= $par == '03' ? 'class="active"' : '' ?>><a href="index.php?par=03">Ultimas 6 horas</a></li>
                        <li <?= $par == '04' ? 'class="active"' : '' ?>><a href="index.php?par=04">Ultimas 24 horas</a></li>
                        <li <?= $par == '05' ? 'class="active"' : '' ?>><a href="index.php?par=05">Customizado</a></li>
                    </ul>
                </div>

                <div class="col-sm-9 col-sm-offset-3 col-md-10 col-md-offset-2 main">
                    <h1 class="page-header">Medi&ccedil;&otilde;es</h1>
                    <div class="row placeholders">
                        <div id="relogios"><h1>Medições</h1></div>
                        <div class="col-xs-6 col-sm-3 placeholder">
                            <div id="relogio_temperatura" ></div>
                            <h4>Temperatura</h4>
                            <span class="text-muted" id="lbl_temperatura"> <?= $leitor->temperatura ?> </span>
                        </div>
                        <div class="col-xs-6 col-sm-3 placeholder">
                            <div id="relogio_luminosidade"></div>
                            <h4>Luminosidade</h4>
                            <span class="text-muted" id="lbl_luminosidade"><?= $leitor->luminosidade ?></span>
                        </div>
                        <div class="col-xs-6 col-sm-2 ">
                            <img width="150px" height="auto" id="img_porta" src="<?= $leitor->porta == 'Aberta' ? 'img/porta_aberta.png' : 'img/porta_fechada.png' ?>" alt="porta"/>
                            <h4>Porta</h4>
                            <span class="text-muted" id="lbl_porta"><?= $leitor->porta ?></span>
                        </div>
                        <div class="col-xs-6 col-sm-2 placeholder">
                            <img data-src="holder.js/200x200/auto/vine" class="img-responsive" alt="Generic placeholder thumbnail">
                            <h4>Presença</h4>
                            <span class="text-muted" id="lbl_presenca"><?= $leitor->presenca ?></span>
                        </div>
                        <div class="col-xs-6 col-sm-2 placeholder">
                            <img data-src="holder.js/200x200/auto/vine" class="img-responsive" alt="Generic placeholder thumbnail">
                            <h4>Lâmpada</h4>
                            <span class="text-muted" id="lbl_lampada"><?= $leitor->lampada ?></span>
                        </div>
                    </div>      
                    <div id="charts">
                        <img id="progresso" src="img/loading-bar-gif.gif" style="visibility: hidden; z-index: 9999; position: absolute;" />
                        <?php if (isset($tabela_temperatura)): ?>                        
                            <div class="chart_tempo_real" id="chart_temperatura" ></div>
                        <?php endif; ?>
                        <?php if (isset($tabela_presenca)): ?>                        
                            <div class="chart_tempo_real" id="chart_presenca" ></div>
                        <?php endif; ?>                        
                        <?php if (isset($tabela_luminosidade)): ?>                        
                            <div class="chart_tempo_real" id="chart_luminosidade" ></div>
                        <?php endif; ?>                                                
                        <?php if (isset($tabela_porta)): ?>                        
                            <div class="chart_tempo_real" id="chart_porta" ></div>
                        <?php endif; ?>                             
                        <?php if (isset($tabela_lampada)): ?>                        
                            <div class="chart_tempo_real" id="chart_lampada" ></div>
                        <?php endif; ?>   
                    </div>
                </div>
            </div>
        </div>

        <!-- Bootstrap core JavaScript
        ================================================== -->
        <!-- Placed at the end of the document so the pages load faster -->
        <script src="js/jquery.min.js"></script>
        <script src="js/jquery.timers-1.0.0.js"></script>
        <script src="js/bootstrap.min.js"></script>
        <script src="js/docs.min.js"></script>
        <script src="js/globalize.min.js"></script>
        <script src="js/dx.chartjs.js"></script>

        <script>
<?php if (isset($tabela_temperatura)) : ?>
                var dataTemperatura = [
    <?= $tabela_temperatura ?>
                ];
                $("#chart_temperatura").dxChart({
                    dataSource: dataTemperatura,
                    title: 'Temperatura',
                    size: {
                        height: 300
                    },
                    animation: {
                        duration: 0,
                        enabled: false
                    },
                    series: {
                        argumentField: 'arg',
                        valueField: 'val',
                        type: 'spline'
                    },
                    /*argumentAxis: {
                     grid: {
                     visible: true
                     }
                     },*/
                    /*tooltip: {
                     enabled: true,
                     color: '#A6C567'
                     },*/
                    legend: {
                        visible: false
                    },
                    valueAxis: {
                        min: 16,
                        max: 24,
                        label: {
                            customizeText: function() {
                                return this.valueText + '&#176C';
                            }
                        }
                    }
                });
<?php endif; ?>

<?php if (isset($tabela_luminosidade)) : ?>
                var dataLuminosidade = [
    <?= $tabela_luminosidade ?>
                ];
                $("#chart_luminosidade").dxChart({
                    dataSource: dataLuminosidade,
                    title: 'Luminosidade',
                    size: {
                        height: 300
                    },
                    animation: {
                        duration: 0,
                        enabled: false
                    },
                    series: {
                        argumentField: 'arg',
                        valueField: 'val'
                                //type: 'line'
                    },
                    /*argumentAxis: {
                     grid: {
                     visible: true
                     }
                     },
                     tooltip: {
                     enabled: true,
                     color: '#A6C567'
                     },*/
                    legend: {
                        visible: false
                    },
                    valueAxis: {
                        //min: 500,
                        label: {
                            customizeText: function() {
                                return this.valueText;
                            }
                        }
                    }
                });
<?php endif; ?>

<?php if (isset($tabela_porta)) : ?>
                var dataPorta = [
    <?= $tabela_porta ?>
                ];
                $("#chart_porta").dxChart({
                    dataSource: dataPorta,
                    title: 'Sensor da Porta',
                    size: {
                        height: 300
                    },
                    series: {
                        argumentField: 'arg',
                        valueField: 'val',
                        type: 'bar'
                    },
                    animation: {
                        duration: 0,
                        enabled: false
                    },
                    legend: {
                        visible: false
                    },
                    valueAxis: {
                        min: 0,
                        max: 1,
                        label: {
                            customizeText: function() {
                                return this.valueText;
                            }
                        }
                    }
                });
<?php endif; ?>

<?php if (isset($tabela_presenca)) : ?>
                var dataPresenca = [
    <?= $tabela_presenca ?>
                ];
                $("#chart_presenca").dxChart({
                    dataSource: dataPresenca,
                    title: 'Sensor de Presença',
                    size: {
                        height: 300
                    },
                    series: {
                        argumentField: 'arg',
                        valueField: 'val',
                        type: 'bar'
                    },
                    legend: {
                        visible: false
                    },
                    animation: {
                        duration: 0,
                        enabled: false
                    },
                    valueAxis: {
                        min: 0,
                        max: 1,
                        label: {
                            customizeText: function() {
                                return '';
                            }
                        }
                    }
                });
<?php endif; ?>

<?php if (isset($tabela_lampada)) : ?>
                var dataLampada = [
    <?= $tabela_lampada ?>
                ];
                $("#chart_lampada").dxChart({
                    dataSource: dataLampada,
                    title: 'Status da lâmpada',
                    size: {
                        height: 300
                    },
                    series: {
                        argumentField: 'arg',
                        valueField: 'val',
                        type: 'bar'
                    },
                    legend: {
                        visible: false
                    },
                    animation: {
                        duration: 0,
                        enabled: false
                    },
                    valueAxis: {
                        min: 0,
                        max: 1,
                        label: {
                            customizeText: function() {
                                return '';
                            }
                        }
                    }
                });
<?php endif; ?>

            $("#relogio_temperatura").dxCircularGauge({
                title: "Termometro",
                scale: {
                    startValue: 13,
                    endValue: 50,
                    majorTick: {
                        color: 'black'
                    },
                    minorTick: {
                        visible: true,
                        color: 'black'
                    },
                    label: {
                        format: 'number'
                    }
                },
                rangeContainer: {
                    backgroundColor: 'none',
                    ranges: [{
                            startValue: 13,
                            endValue: 18,
                            color: '#A6C567'
                        }, {
                            startValue: 18,
                            endValue: 22,
                            color: '#FCBB69'
                        }, {
                            startValue: 22,
                            endValue: 50,
                            color: '#E19094'
                        }]
                },
                value: <?= $leitor->temperatura ?>,
                subvalues: [18]
            });

            $("#relogio_luminosidade").dxCircularGauge({
                title: "Luminosidade",
                scale: {
                    startValue: 600,
                    endValue: 1024,
                    majorTick: {
                        color: 'black'
                    },
                    minorTick: {
                        visible: true,
                        color: 'black'
                    },
                    label: {
                        format: 'number'
                    }
                },
                rangeContainer: {
                    backgroundColor: 'none',
                    ranges: [{
                            startValue: 600,
                            endValue: 930,
                            color: '#A6C567'
                        }, {
                            startValue: 930,
                            endValue: 1024,
                            color: '#E19094'
                        }]
                },
                value: <?= $leitor->luminosidade ?>,
                subvalues: [930]
            });
            var ultimaMedida;
            function addData(chart, arg, val, b) {

                if (arg !== ultimaMedida) {
                    if (b) {
                        ultimaMedida = arg;
                    }
                    var datasource = chart._dataSource;
                    var itens = datasource._items;


                    var obj = {
                        arg: arg.split(' ')[1],
                        val: val
                    };

                    if (itens.length > 60) {
                        itens.shift();
                    }

                    itens.push(obj);

                    chart.beginUpdate();

                    chart.option({
                        commonSeriesSettings: {
                            point: {visible: false}
                        }
                    });
                    chart.option('dataSource', itens);
                    chart.endUpdate();
                }
            }

            $("#relogios").everyTime(1000, function(i) {
                $.ajax({
                    url: "atualiza_relogios.php",
                    cache: false,
                    success: function(html) {
                        var lista = html.split(',');
                        $("#lbl_temperatura").text(lista[4]);
                        $("#relogio_temperatura").dxCircularGauge({
                            value: lista[4]
                        });
                        $("#lbl_lampada").text(lista[0]);
                        $("#lbl_luminosidade").text(lista[1]);
                        $("#relogio_luminosidade").dxCircularGauge({
                            value: lista[1]
                        });
                        $("#lbl_porta").text(lista[2]);
                        $("#img_porta").attr('src', lista[2] === 'Aberta' ? 'img/porta_aberta.png' : 'img/porta_fechada.png')
                        $("#lbl_presenca").text(lista[3]);
<?php if ($par == 00) : ?>
                            addData($("#chart_temperatura").dxChart('instance'), lista[5], lista[4]), false;
                            addData($("#chart_luminosidade").dxChart('instance'), lista[5], lista[1], false);
                            addData($("#chart_presenca").dxChart('instance'), lista[5], lista[3] === 'Não Detectado' ? 0 : 1, true);
<?php endif; ?>

                    }
                });
            });

<?php if ($par == 99) : ?>
                $("#charts").everyTime(10000, function(i) {
                    $("#progresso").show();
                    $(".chart_tempo_real").dxChart('instance').beginUpdate();
                    // Temperatura
                    $.ajax({
                        url: "grafico_tempo_real.php?par=t",
                        dataType: "json",
                        contentType: "application/json",
                        async: false,
                        success: function(ret) {
                            $("#chart_temperatura").dxChart({
                                dataSource: ret,
                                animation: {
                                    duration: 0,
                                    enabled: false
                                }
                            });
                        },
                    });

                    // Luminosidade
                    $.ajax({
                        url: "grafico_tempo_real.php?par=l",
                        dataType: "json",
                        contentType: "application/json",
                        async: false,
                        success: function(ret) {
                            $("#chart_luminosidade").dxChart({
                                dataSource: ret,
                                animation: {
                                    duration: 0,
                                    enabled: false
                                }
                            });
                        },
                    });

                    // Presença
                    $.ajax({
                        url: "grafico_tempo_real.php?par=r",
                        dataType: "json",
                        contentType: "application/json",
                        async: false,
                        success: function(ret) {
                            $("#chart_presenca").dxChart({
                                dataSource: ret,
                                animation: {
                                    duration: 0,
                                    enabled: false
                                }
                            });
                        },
                    });

                    // Porta
                    $.ajax({
                        url: "grafico_tempo_real.php?par=q",
                        dataType: "json",
                        contentType: "application/json",
                        async: false,
                        success: function(ret) {
                            $("#chart_porta").dxChart({
                                dataSource: ret,
                                animation: {
                                    duration: 0,
                                    enabled: false
                                }
                            });
                        },
                    });

                    // Lâmpada
                    $.ajax({
                        url: "grafico_tempo_real.php?par=s",
                        dataType: "json",
                        contentType: "application/json",
                        async: false,
                        success: function(ret) {
                            $("#chart_lampada").dxChart({
                                dataSource: ret,
                                animation: {
                                    duration: 0,
                                    enabled: false
                                }
                            });
                        },
                    });

                    $(".chart_tempo_real").dxChart('instance').endUpdate();
                    $("#progresso").hide();
                }
                );


<?php endif; ?>

        </script>
    </body>
</html>
