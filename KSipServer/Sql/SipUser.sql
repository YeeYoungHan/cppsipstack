CREATE TABLE SipUser(
  Id VARCHAR(20) NOT NULL,
  PassWord VARCHAR(20),
  DND CHAR(1),
  CallForward VARCHAR(100),
  PRIMARY KEY (`Id`)
);
