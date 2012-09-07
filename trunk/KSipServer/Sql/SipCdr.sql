CREATE TABLE SipCdr(
  FromId VARCHAR(50) NOT NULL,
  ToId VARCHAR(50) NOT NULL,
  CallId VARCHAR(255) NOT NULL,
  InviteTime VARCHAR(14) NOT NULL,
  StartTime VARCHAR(14) NOT NULL,
  EndTime VARCHAR(14) NOT NULL,
  SipStatus INT,
  PRIMARY KEY ( FromId, ToId, CallId, InviteTime )
);
