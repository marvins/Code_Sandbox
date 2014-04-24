#    File:    Calculator.py
#    Author:  Marvin Smith
#    Date:    4/18/2014
#    
#    Purpose:  Simple Calculator Add-On
#

#  PyQt4
from PyQt4 import QtGui
from PyQt4.QtCore import *

#  Plugin Base
import PluginBase

#  Calculator Plug-In
class Calculator(PluginBase.PluginBase):
	
	#  Infix stack
	infixStack  = []

	#  Cursor to where in the dialog we are.  
	infixCursor = 0;

	#  This maps operators to their matching priority
	operatorDictionary = { '+':2, '-':2, '*':4, '/':4, '^':5 };

	
	#  Constructor
	def __init__(self, parent = None):
		
		#  Create parent
		PluginBase.PluginBase.__init__(self, parent);

		#  Create Base Layout
		self.mainLayout = QtGui.QVBoxLayout();

		#  Create the main UI
		self.initUI()

		#  Set the main layout
		self.setLayout( self.mainLayout );


	#  Open the dialog box
	def openDialog(self):
		
		self.show();
		 
	
	#  Initialize the User Interface
	def initUI(self):
		
		#  set the title
		self.setWindowTitle('Calculator');
		
		#  Create the main layout
		self.mainLayout = QtGui.QVBoxLayout();

		#  Create the main editor
		self.buildCalculatorDisplay();
	
		#  Create button panel
		self.buildButtonPanel();

		#  Set the layout
		self.setLayout(self.mainLayout);
	


	#  Create the main display
	def buildCalculatorDisplay(self):

		#  Create main widget
		self.displayWidget = QtGui.QWidget();
		self.displayLayout = QtGui.QVBoxLayout();

		#  Create the editor
		self.displayEdit   = QtGui.QTextEdit();
		self.displayEdit.setReadOnly(True);
		self.displayEdit.installEventFilter(self);
		self.displayLayout.addWidget( self.displayEdit );

		#  Grab the cursor
		self.displayCursor = self.displayEdit.textCursor();
		self.displayCursor.insertText("Command > ");

		#  Set the layout
		self.displayWidget.setLayout( self.displayLayout );
		self.mainLayout.addWidget(self.displayWidget);

	
	#  Create the button panel
	def buildButtonPanel(self):
		
		# create main widget
		self.buttonPanelWidget = QtGui.QWidget();
		self.buttonPanelLayout = QtGui.QGridLayout();

		# process
		self.buttonPanelEnterButton = QtGui.QToolButton();
		self.buttonPanelEnterButton.setText("Enter");
		self.buttonPanelEnterButton.setFixedWidth(100);
		self.buttonPanelEnterButton.setFixedHeight(60);
		self.buttonPanelEnterButton.clicked.connect(self.calculate);
		self.buttonPanelLayout.addWidget(self.buttonPanelEnterButton, 0, 0 );


		#  clear
		self.buttonPanelClearButton = QtGui.QToolButton();
		self.buttonPanelClearButton.setText('Clear');
		self.buttonPanelClearButton.setFixedWidth(100);
		self.buttonPanelClearButton.setFixedHeight(60);
		self.buttonPanelClearButton.clicked.connect(self.clearDisplay);
		self.buttonPanelLayout.addWidget(self.buttonPanelClearButton, 0, 1);
		
		#  set widget
		self.buttonPanelWidget.setLayout(self.buttonPanelLayout);
		self.mainLayout.addWidget(self.buttonPanelWidget);
	

	# Process Keyboard Inputs
	def eventFilter(self, obj, event):

		if event.type() == QEvent.KeyPress:
			
			# Add number keys
			if event.key() >= ord('0') and event.key() <= ord('9'):
				self.displayCursor.insertText(chr(event.key()));
				self.infixStack.append(chr(event.key()));
				self.infixCursor += 1
			
			#  Add main operators
			elif event.key() == ord('*') or event.key() == ord('/') or event.key() == ord('+') or event.key() ==  ord('-'):
				self.displayCursor.insertText(chr(event.key()));
				self.infixStack.append(chr(event.key()));
				self.infixCursor += 1

			#  Add Parenthesis
			elif event.key() == ord('(') or event.key() == ord(')'):
				self.displayCursor.insertText(chr(event.key()));
				self.infixStack.append(chr(event.key()));
				self.infixCursor += 1
			
			#  Add exponent
			elif event.key() == ord('^'):
				self.displayCursor.insertText(chr(event.key()));
				self.infixStack.append(chr(event.key()));
				self.infixCursor += 1
		
			#  Process Data
			elif event.key() == Qt.Key_Enter or event.key() == 13:
				self.calculate();
			
			#  Process a spacebar
			elif event.key() == ord(' '):
				self.displayCursor.insertText(chr(event.key()));
				self.infixStack.append(chr(event.key()));
				self.infixCursor += 1
				
			#  Remove a key
			elif event.key() == Qt.Key_Backspace:
				if self.infixCursor > 0:
					self.infixCursor -= 1
					self.infixStack = self.infixStack[:self.infixCursor] + self.infixStack[(self.infixCursor+1):];
					self.displayCursor.deletePreviousChar();


		return False;


	def calculate(self):

		#  Convert the infix notation to postfix notation
		postfixStack = self.convertInfixToPostfix(self.cleanInfix(self.infixStack));
		
		#  Compute a solution
		solution = self.solvePostfix(postfixStack);

		#  Set the console to the solution
		self.displayCursor.insertText("\nSolution: " + solution + "\n");
		self.displayCursor.insertText("\nCommand > ");
		self.displayEdit.ensureCursorVisible();

		#  Reset the stack
		self.infixStack = []
		self.infixCursor = 0;

		

	def cleanInfix(self, data):
		
		output = []
		for x in data:
			if self.isValidCharacter(x):
				output.append(x);

		return output;

	
	def isValidCharacter(self, data):
		if self.isValidOperandCharacter(data):
			return True;
		elif self.isValidOperatorCharacter(data):
			return True;
		else:
			return False;
	

	def isValidOperandCharacter(self, data):
		if data >= '0' and data <= '9':
			return True;
		return False;
	

	def isValidOperatorCharacter(self, data):
		if data == '+' or data == '-' or data == '*' or data == '/':
			return True;
		return False;


	def convertInfixToPostfix( self, infixData ):
		
		#  Create output
		postfixData = [];

		#  List of operators
		operatorStack = [];
		tempOperand = ''

		for i in xrange(0, len(infixData)):
			
			# if we have an integer, add it to the temporary value
			if self.isValidOperandCharacter(infixData[i]):	
				tempOperand += infixData[i];

			#  Otherwise, annotate the end of the operator and add to the stack
			else:
				postfixData.append(tempOperand);
				tempOperand = ''

				# If the item is an operator, then add it to the operator stack
				if len(operatorStack) == 0 or self.operatorALessThanB(operatorStack[-1], infixData[i]) == True:
					operatorStack.append(infixData[i]);
				else:
					while self.operatorALessThanB(operatorStack[-1], infixData[i]) == False and len(operatorStack) > 0:
						postfixData.append(operatorStack[-1]);
						operatorStack = operatorStack[0:len(operatorStack)-1];

					postfixData.append(infixData[i]);

		#  Add any remaining operands
		if not tempOperand == '':
			postfixData.append(tempOperand);
			tempOperand=''

		#  Append the operator stack to the output
		while len(operatorStack) > 0:
			postfixData.append(operatorStack[-1])
			operatorStack = operatorStack[:len(operatorStack)-1];
		

		print('PostfixData: ', postfixData)
		return postfixData


	def operatorALessThanB( self, a, b):
		if self.operatorDictionary[a] < self.operatorDictionary[b]:
			return True;
		else:
			return False;


	def solvePostfix(self, postfixStack):
		
		#  Start popping values until we hit an operator
		operandStack = []
		
		postfixData = postfixStack;
		while len(postfixData) > 0:
			
			#  Pop next value
			value = postfixData[0];
			postfixData = postfixData[1:];
			
			#  If the value is an operand, add it to the stack
			if self.isOperand(value):
				operandStack.append(float(value));

			#  Otherwise, apply the operator
			else:
				op1 = operandStack[-2];
				op2 = operandStack[-1];
				operandStack = operandStack[:len(operandStack)-2]


				result = self.applyOperation( op1, op2, value);
				operandStack.append(result);
		
		return str(operandStack[0]);




	def applyOperation(self, op1, op2, value):
		
		if value == '*':
			return op1*op2;
		if value == '/':
			return op1/op2;
		if value == '+':
			return op1+op2;
		if value == '-':
			return op1-op2;
		raise Exception('Unknown operator: ' + value );

	def isOperand(self, data):
		
		try:
			value = float(data);
		except:
			return False;
		return True;
	
	def isOperator(self,data):
		return self.isValidOperatorCharacter(data);


	def getButtonIconSize(self):
		return QSize(70,70);

	def getButtonText(self):
		return 'Calculator'
	
	def getButtonIconPath(self):
		return 'calculator.png';
	

	#  Clear all contents on the display
	def clearDisplay(self):
		
		#  Wipe out the display
		self.displayEdit.clear();
		self.displayCursor = self.displayEdit.textCursor();

