CREATE TABLE SipServer(
  Id VARCHAR(20) NOT NULL,
  Ip VARCHAR(20) NOT NULL,
  Port INT,
  Domain VARCHAR(50),
  UserId VARCHAR(50),
  PassWord VARCHAR(50),
  PRIMARY KEY (`Id`)
);

INSERT INTO SipServer( Id, Ip, Port, Domain, UserId, PassWord ) VALUES( 'freeswitch', '192.168.89.129', 5060, '', '1000', '1234' );
