CREATE TABLE RoutePrefix(
  Id VARCHAR(20) NOT NULL,
  Prefix VARCHAR(20) NOT NULL,
  DeletePrefix CHAR(1) NOT NULL,
  PRIMARY KEY (Id, Prefix)
);

INSERT INTO RoutePrefix( Id, Prefix, DeletePrefix ) VALUES( 'freeswitch', '0', 'Y' );
