CREATE TABLE StatsMethodUserAgent(
  Date CHAR(8) NOT NULL,
  Method VARCHAR(20) NOT NULL,
  UserAgent VARCHAR(100) NOT NULL,
  Count BIGINT(20),
  PRIMARY KEY( Date, Method, UserAgent )
);
