CREATE TABLE StatsMethodIp(
  Date CHAR(8) NOT NULL,
  Method VARCHAR(20) NOT NULL,
  Ip VARCHAR(15) NOT NULL,
  Count BIGINT(20),
  PRIMARY KEY( Date, Method, Ip )
);
