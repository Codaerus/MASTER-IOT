let temp = 30;
let humi = 70;
let out = "INSERT INTO sensores (id, tiempo, temperatura, humedad) ";
out = out + "VALUES (NULL, CURRENT_TIMESTAMP," + temp + ","+ humi+ ");";
msg.topic = out;
return msg;
