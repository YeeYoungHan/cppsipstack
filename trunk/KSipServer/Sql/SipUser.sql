CREATE TABLE SipUser(
  Id VARCHAR(20) NOT NULL,
  PassWord VARCHAR(20),
  DND CHAR(1),
  CallForward VARCHAR(100),
  GroupId VARCHAR(20),
  PRIMARY KEY (`Id`)
);

INSERT INTO SipUser( Id, PassWord, DND, CallForward, GroupId ) VALUES( '1000', '1234', 'N', '', '' );
INSERT INTO SipUser( Id, PassWord, DND, CallForward, GroupId ) VALUES( '1001', '1234', 'N', '', '' );
INSERT INTO SipUser( Id, PassWord, DND, CallForward, GroupId ) VALUES( '1002', '1234', 'Y', '', '' );
INSERT INTO SipUser( Id, PassWord, DND, CallForward, GroupId ) VALUES( '1003', '1234', 'N', '1001', '' );
