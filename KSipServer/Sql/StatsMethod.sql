CREATE TABLE StatsMethod(
  Date CHAR(8) NOT NULL,
  Method VARCHAR(20) NOT NULL,
  Count BIGINT(20),
  PRIMARY KEY( Date, Method )
);
