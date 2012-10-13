<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=EUC-KR">
<title>Search CDR</title>
</head>
<body>
<b>Search CDR</b>
<br><br>
<form name="user" method="post" action="CdrSearchList.jsp">
<table>
	<tr><td>
		<table border=1 cellspacing=0 cellpadding=5>
			<tr>
				<td width="100"><b>From</b></td>
				<td width="400"><input type="text" name="from" ></td>
			</tr>
			<tr>
				<td><b>To</b></td>
				<td><input type="text" name="to" ></td>
			</tr>
			<tr>
				<td><b>CallId</b></td>
				<td><input type="text" name="callid" ></td>
			</tr>		
			<tr>
				<td><b>InviteTime</b></td>
				<td><input type="text" name="invitetimestart" >&nbsp;~&nbsp;<input type="text" name="invitetimeend" ></td>
			</tr>		
		</table>
	</td></tr>
	<tr><td align="right">
		<input type="submit" value="SEARCH">&nbsp;&nbsp;<input type="button" value="BACK" onClick="history.go(-1)">
	</td></tr>
</table>	
</form>
</body>
</html>