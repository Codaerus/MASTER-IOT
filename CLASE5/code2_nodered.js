let temp = msg.payload.split(",")[0];
let humi = msg.payload.split(",")[1];
let out = "INSERT INTO sensores (id, tiempo, temperatura, humedad) ";
out = out + "VALUES (NULL, CURRENT_TIMESTAMP," + temp + ","+ humi+ ");";
msg.topic = out;
return msg;
