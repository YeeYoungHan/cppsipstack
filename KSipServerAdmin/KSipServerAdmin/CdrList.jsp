<%@ page language="java" contentType="text/html; charset=EUC-KR" pageEncoding="EUC-KR"%>
<%@ page import="java.sql.*" %>
<%!
	public String GetDateString( String strDate )
	{
		if( strDate == null ) return "";
		if( strDate.length() == 14 )
		{
			String strYear = strDate.substring( 0, 4 );
	    String strMonth = strDate.substring( 4, 6 );
	    String strDay = strDate.substring( 6, 8 );
	    
	    String strHour = strDate.substring( 8, 10 );
	    String strMin = strDate.substring( 10, 12 );
	    String strSec = strDate.substring( 12, 14 );
	    
	    return String.format( "%s-%s-%s %s:%s:%s", strYear, strMonth, strDay, strHour, strMin, strSec );
		}

		return "";
	}
	
	public int GetInt( String strInput, int iDefaultValue )
	{
		if( strInput == null ) return iDefaultValue;
		
		return Integer.parseInt( strInput );
	}
%>
<%@ include file="DbInfo.jsp" %>
<%
	int iPageRowNum = GetInt( request.getParameter( "rowcount" ), 30 );
	int iPage = GetInt( request.getParameter( "page" ), 0 );
	int iStartPos = 0;
	int iRowCount = 0;

	if( iPage > 0 )
	{
		--iPage;
		iStartPos = iPage * iPageRowNum;
	}
	
	Connection m_clsDbConn = null;
	
	try
	{
		Class.forName("com.mysql.jdbc.Driver");
		
		m_clsDbConn = DriverManager.getConnection( "jdbc:mysql://localhost/ksipserver", m_strDbUserId, m_strDbPassWord );
		Statement clsStmt = m_clsDbConn.createStatement( );
		String strSQL = "SELECT count(*) FROM sipcdr";
		
		ResultSet clsRS = clsStmt.executeQuery( strSQL );
		if( clsRS.next( ) )
		{
			iRowCount = clsRS.getInt( 1 );
		}
		
		clsRS.close();
		clsStmt.close( );
	}
	catch( Exception e )
	{
		e.printStackTrace( );
	}
%>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=EUC-KR">
<title>CDR List</title>
</head>
<body>
<b>CDR List</b><br>
<br>
<table>
	<tr><td>

<table border=1 cellspacing=0 cellpadding=5>
	<tr bgcolor="#ffcccc" >
		<td width="60" align="center"><b>From</b></td>
		<td width="60" align="center"><b>To</b></td>
		<td width="100" align="center"><b>CallId</b></td>
		<td width="150" align="center"><b>InviteTime</b></td>
		<td width="150" align="center"><b>StartTime</b></td>
		<td width="150" align="center"><b>EndTime</b></td>
		<td width="60" align="center"><b>SipStatus</b></td>
	</tr>
<%
	if( m_clsDbConn != null )
	{
		String strSQL = "SELECT FromId, ToId, CallId, InviteTime, StartTime, EndTime, SipStatus "
				+ "FROM sipcdr ORDER BY InviteTime DESC LIMIT ?,?";
		
		PreparedStatement clsStmt = m_clsDbConn.prepareStatement( strSQL );
		clsStmt.setInt( 1, iStartPos );
		clsStmt.setInt( 2, iPageRowNum );
		
		ResultSet clsRS = clsStmt.executeQuery( );
		String strBgColor;
		int	iRow = 0;
		
		while( clsRS.next() )
		{
			++iRow;
			if( iRow % 2 == 0 )
			{
				strBgColor = "bgcolor=\"#ffeeee\"";
			}
			else
			{
				strBgColor = "";
			}
			
			String strInviteTime = GetDateString( clsRS.getString( 4 ) );
			String strStartTime = GetDateString( clsRS.getString( 5 ) );
			String strEndTime = GetDateString( clsRS.getString( 6 ) );
%>
	<tr <%= strBgColor %>>
		<td><%= clsRS.getString( 1 ) %></td>
		<td><%= clsRS.getString( 2 ) %></td>
		<td><%= clsRS.getString( 3 ) %></td>
		<td><%= strInviteTime %></td>
		<td><%= strStartTime %></td>
		<td><%= strEndTime %></td>
		<td align="center"><%= clsRS.getInt( 7 ) %></td>
	</tr>
<%			
		}
		
		clsRS.close( );
		clsStmt.close( );
	}
%>
</table>
</td></tr>

<tr><td align="center">
<%
	if( iRowCount > iPageRowNum )
	{
		int iPageCount = iRowCount / iPageRowNum;
		if( iRowCount % iPageRowNum > 0 ) ++iPageCount;
		
		for( int i = 0; i < iPageCount; ++i )
		{
			int iNum = i + 1;
			
			if( i == iPage )
			{
				out.println( "<b>" + iNum + "</b>&nbsp;" );
			}
			else
			{
				out.println( "<a href=\"CdrList.jsp?page=" + iNum + "\">" + iNum + "</a>&nbsp;" );
			}
		}
	}
%>
</td></tr>

</table>

</body>
</html>
<%
	try
	{
		if( m_clsDbConn != null ) m_clsDbConn.close();
	}
	catch( Exception e )
	{
		e.printStackTrace( );
	}
%>