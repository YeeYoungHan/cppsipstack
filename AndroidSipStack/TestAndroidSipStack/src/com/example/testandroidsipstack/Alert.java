package com.example.testandroidsipstack;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;

public class Alert
{
	public static void ShowError( Context clsContext, String strMessage )
	{
		new AlertDialog.Builder( clsContext ).setTitle( "Error" ).setMessage( strMessage ).setPositiveButton( "OK"
				, new DialogInterface.OnClickListener( )
					{
						public void onClick( DialogInterface dialog, int which )
						{
							dialog.dismiss( );
						}
					} ).show( );
	}
}
