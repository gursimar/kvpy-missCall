VERSION 5.00
Object = "{648A5603-2C6E-101B-82B6-000000000014}#1.1#0"; "MSCOMM32.OCX"
Object = "{3B7C8863-D78F-101B-B9B5-04021C009402}#1.2#0"; "RICHTX32.OCX"
Begin VB.Form Form1 
   Caption         =   "Data Logger - simar"
   ClientHeight    =   9285
   ClientLeft      =   60
   ClientTop       =   510
   ClientWidth     =   8580
   LinkTopic       =   "Form1"
   ScaleHeight     =   9285
   ScaleWidth      =   8580
   StartUpPosition =   3  'Windows Default
   Begin VB.CommandButton Command4 
      Caption         =   "TEST"
      Height          =   1335
      Left            =   11520
      TabIndex        =   7
      Top             =   5400
      Width           =   735
   End
   Begin VB.Frame Frame2 
      Caption         =   "Output"
      Height          =   9135
      Left            =   120
      TabIndex        =   4
      Top             =   0
      Width           =   8295
      Begin VB.CommandButton Command3 
         Caption         =   "Clear"
         Height          =   375
         Left            =   2760
         TabIndex        =   6
         Top             =   8520
         Width           =   1935
      End
      Begin RichTextLib.RichTextBox RichTextBox1 
         Height          =   7935
         Left            =   240
         TabIndex        =   5
         Top             =   360
         Width           =   7815
         _ExtentX        =   13785
         _ExtentY        =   13996
         _Version        =   393217
         Enabled         =   -1  'True
         ReadOnly        =   -1  'True
         ScrollBars      =   2
         Appearance      =   0
         TextRTF         =   $"Smart CAR data loger.frx":0000
         BeginProperty Font {0BE35203-8F91-11CE-9DE3-00AA004BB851} 
            Name            =   "Arial"
            Size            =   12
            Charset         =   0
            Weight          =   400
            Underline       =   0   'False
            Italic          =   0   'False
            Strikethrough   =   0   'False
         EndProperty
      End
   End
   Begin VB.Frame Frame1 
      Caption         =   "Settings"
      Height          =   1095
      Left            =   8760
      TabIndex        =   0
      Top             =   3720
      Width           =   7935
      Begin VB.CommandButton Command1 
         Caption         =   "ReConnect"
         Height          =   375
         Left            =   4440
         TabIndex        =   3
         Top             =   360
         Width           =   2415
      End
      Begin VB.TextBox Text1 
         Height          =   375
         Left            =   2040
         TabIndex        =   1
         Text            =   "2"
         Top             =   360
         Width           =   1455
      End
      Begin VB.Label Label1 
         AutoSize        =   -1  'True
         Caption         =   "COMM PORT"
         Height          =   195
         Left            =   600
         TabIndex        =   2
         Top             =   480
         Width           =   990
      End
   End
   Begin MSCommLib.MSComm Simar 
      Left            =   8040
      Top             =   120
      _ExtentX        =   1005
      _ExtentY        =   1005
      _Version        =   393216
      CommPort        =   2
      DTREnable       =   -1  'True
      RThreshold      =   51
      SThreshold      =   51
   End
End
Attribute VB_Name = "Form1"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False
Option Explicit
Dim Position As Integer
Dim Startdata As Integer
Dim Enddata As Integer
Dim Data As String
Dim Senarray(8) As Integer
Dim NoData As Integer

'Excel variables
Dim oXL As Excel.Application
Dim oWB As Excel.Workbook
Dim oSheet As Excel.Worksheet
Dim oRng As Excel.Range


Private Sub Command1_Click()
Simar.PortOpen = False
Simar.CommPort = CInt(Text1.Text)
Simar.Settings = "9600,N,8,1"
Simar.PortOpen = True
RichTextBox1.SetFocus
End Sub


Private Sub Command2_Click()

End Sub

Private Sub Command3_Click()    'CLEAR

RichTextBox1.Text = ""
'MsgBox CInt(vbCrLf)

End Sub

Private Sub Command4_Click()    'TEST

'RichTextBox1.SelIndent
'RichTextBox1.SelLength
'RichTextBox1.SelStart
'RichTextBox1.SelText


'RichTextBox1.SelBullet = 1
'RichTextBox1.SelColor = 74

'RichTextBox1.SelStart = 5
'RichTextBox1.SetFocus


Position = InStr(Position + 1, RichTextBox1.Text, "$", vbTextCompare)

'If Position > 0 Then
'    RichTextBox1.SelStart = Position - 1
'    RichTextBox1.SelLength = 1 'Len(Text1.Text)
    'MsgBox position
    Startdata = Position
'    RichTextBox1.SetFocus
'End If

Position = Position + 1
Position = InStr(Position + 1, RichTextBox1.Text, "$", vbTextCompare)

'If Position > 0 Then
'    RichTextBox1.SelStart = Position - 1
'    RichTextBox1.SelLength = 1 'Len(Text1.Text)
    'MsgBox position
    Enddata = Position
'   RichTextBox1.SetFocus
'End If

Data = Mid(RichTextBox1.Text, Startdata + 1, Enddata - Startdata - 1)
'MsgBox Data

Dim start As Integer
Dim i As Integer

For i = 0 To 7
    Senarray(i) = Mid(Data, start + 1, 3)
    start = start + 6
    'MsgBox Senarray(i)
Next


End Sub

Private Sub Form_Load()
   ' Start Excel and get Application object.
      Set oXL = CreateObject("Excel.Application")
      oXL.Visible = True
      
   ' Get a new workbook.
      Set oWB = oXL.Workbooks.Add
      Set oSheet = oWB.ActiveSheet
      
         ' Add table headers going cell by cell.
      oSheet.Cells(1, 1).Value = "Sensor 7"
      oSheet.Cells(1, 2).Value = "Sensor 6"
      oSheet.Cells(1, 3).Value = "Sensor 5"
      oSheet.Cells(1, 4).Value = "Sensor 4"
      oSheet.Cells(1, 5).Value = "Sensor 3"
      oSheet.Cells(1, 6).Value = "Sensor 2"
      oSheet.Cells(1, 7).Value = "Sensor 1"
      oSheet.Cells(1, 8).Value = "Sensor 0"
      

   ' Format A1:D1 as bold, vertical alignment = center.
      With oSheet.Range("A1", "H1")
         .Font.Bold = True
         .VerticalAlignment = xlVAlignCenter
      End With
      
DoEvents

Simar.CommPort = 2
Simar.Settings = "9600,N,8,1"
Simar.PortOpen = True
DoEvents
NoData = 2
End Sub

Private Sub Form_Unload(Cancel As Integer)
'MsgBox "Unload"
' Make sure you release object references.
      Set oRng = Nothing
      Set oSheet = Nothing
      Set oWB = Nothing
      Set oXL = Nothing


End Sub

Private Sub RichTextBox1_KeyPress(KeyAscii As Integer)
'MsgBox "Key is pressed" & vbCrLf & "The letter is  " & CStr(KeyAscii)

NoData = NoData + 1
'MsgBox NoData
'MsgBox KeyCode
Simar.Output = Chr(KeyAscii)

End Sub

Private Sub Simar_OnComm()
On Error GoTo ErrHandler
'MsgBox (Simar.InBufferCount)

If Simar.CommEvent = comEvReceive Then
    Dim Buffer As String
    Buffer = Simar.Input
'    DoEvents
    '51 50 49
    'MsgBox CStr(Len(Buffer)) + "  " + Buffer
    RichTextBox1.Text = RichTextBox1.Text + Buffer
    Simar.InBufferCount = 0 'To clear the entire buffer after reading  WORKS BETTER IF COMMENTED
    
    RichTextBox1.SelStart = Len(RichTextBox1.Text)
    
    Position = InStr(Position + 1, RichTextBox1.Text, "$", vbTextCompare)
    Startdata = Position
    
    Position = Position + 1
    Position = InStr(Position + 1, RichTextBox1.Text, "$", vbTextCompare)
    Enddata = Position
    
    Data = Mid(RichTextBox1.Text, Startdata + 1, Enddata - Startdata - 1)
    'MsgBox Data

    Dim start As Integer
    Dim i As Integer
    start = 0
    i = 0
    For i = 0 To 7
        Senarray(i) = Mid(Data, start + 1, 3)
        start = start + 6
        'MsgBox Senarray(i)
    Next
    
    'DoEvents
    FillWorkbook
    DoEvents
    RichTextBox1.SetFocus
  
End If

If Simar.CommEvent = comEvSend Then
End If

If Simar.CommEvent = comEvEOF Then
    MsgBox "END OF FILE"
End If

Exit Sub

ErrHandler:
If Err.Number = 5 Then
Resume Next
Else
MsgBox CStr(Err.Number) + "  " + Err.Description
End If


End Sub

Private Sub FillWorkbook()
      Set oRng = oSheet.Range("A" & CStr(NoData), "H" & CStr(NoData))
 '     DoEvents
      oRng.Value = Senarray
        
      'seems to be working
      'Set oRng = oSheet.Cells(1, NoData + 3)
      'oRng.Formula = "=MAX(A1:A4)"
      
      oXL.Visible = True
      oXL.UserControl = True
      'MsgBox "Hurrah!!"
      Form1.RichTextBox1.SetFocus
      
   Exit Sub
Err_Handler:
      MsgBox Err.Description, vbCritical, "Error: " & Err.Number
End Sub
