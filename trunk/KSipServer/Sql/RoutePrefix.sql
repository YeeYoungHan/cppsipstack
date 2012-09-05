CREATE TABLE RoutePrefix(
  Id VARCHAR(20) NOT NULL,
  Prefix VARCHAR(20) NOT NULL,
  PRIMARY KEY (Id, Prefix)
);

INSERT INTO RoutePrefix( Id, Prefix ) VALUES( 'freeswitch', '0' );
