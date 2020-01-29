const express= require('express');
const app = express()
const SerialPort = require('serialport');
const Readline=SerialPort.parsers.Readline;
const parser = new Readline();

const mySerial=new SerialPort('COM5',{
    baudRate: 9600
})

app.listen(3000,function () {
    console.log("HOLAS");
})

mySerial.pipe(parser)

app.get('/prender', (req,res) =>{
    mySerial.write('"LED1ON"\n')
    res.send("FUNCIONO");
})

app.get('/apagar', (req,res) =>{
    mySerial.write('"LED1OF"\n')
    res.send("APAGADO");
})

