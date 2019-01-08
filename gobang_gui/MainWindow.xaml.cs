using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using System.Runtime.InteropServices;
using System.Threading;

namespace gobang_gui
{
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    /// 
    public partial class MainWindow : Window
    {

        [DllImport(@"GOBANG.dll")]
        public static extern void init(char chessType);

        [DllImport(@"GOBANG.dll")]
        public static extern bool isValid(int row, int col);

        [DllImport(@"GOBANG.dll")]
        public static extern void human(int row, int col);

        [DllImport(@"GOBANG.dll")]
        public static extern void computer();

        [DllImport(@"GOBANG.dll")]
        public static extern int getComputerRow();

        [DllImport(@"GOBANG.dll")]
        public static extern int getComputerCol();


        [DllImport(@"GOBANG.dll")]
        public static extern void updateTurnAfterComputer();


        [DllImport(@"GOBANG.dll")]
        public static extern bool isGameOver(int row, int col);

        [DllImport(@"GOBANG.dll")]
        public static extern bool undoStep();


        [DllImport(@"GOBANG.dll")]
        public static extern bool redoStep();


        [DllImport(@"GOBANG.dll")]
        public static extern void getLastSteps(int[] steps);

        private char humanChessType;
        private bool gameOver;

        public static SynchronizationContext s_SC = SynchronizationContext.Current;

        public MainWindow()
        {
            InitializeComponent();

            //初始化按钮
            RowDefinition topRowDefinition = new RowDefinition();
            topRowDefinition.Height = new GridLength(3, GridUnitType.Star);
            board.RowDefinitions.Add(topRowDefinition);

            for (int i = 0; i < 15; i++)
            {
                RowDefinition rowDefinition = new RowDefinition();
                rowDefinition.Height = new GridLength(1, GridUnitType.Star);
                board.RowDefinitions.Add(rowDefinition);

                ColumnDefinition columnDefinition = new ColumnDefinition();
                columnDefinition.Width = new GridLength(1, GridUnitType.Star);
                board.ColumnDefinitions.Add(columnDefinition);
            }

            Button undoStepButton = new Button();
            undoStepButton.Content = " <- Undo";
            undoStepButton.FontSize = 20;            
            undoStepButton.Click += new RoutedEventHandler(clickUndoStep);
            Grid.SetRow(undoStepButton, 0);
            Grid.SetColumn(undoStepButton, 0);
            Grid.SetColumnSpan(undoStepButton, 5);
            board.Children.Add(undoStepButton);

            Button redoStepButton = new Button();
            redoStepButton.Content = "Redo -> ";
            redoStepButton.FontSize = 20;
            redoStepButton.Click += new RoutedEventHandler(clickRedoStep);
            Grid.SetRow(redoStepButton, 0);
            Grid.SetColumn(redoStepButton, 10);
            Grid.SetColumnSpan(redoStepButton, 5);
            board.Children.Add(redoStepButton);

            for (int i = 1; i < board.RowDefinitions.Count; i++)
                for (int j = 0; j < board.ColumnDefinitions.Count; j++)
                {
                    Button button = new Button();
                    button.FontStretch = new FontStretch();
                    button.FontSize = 20;
                    button.Name = toName(i, j);
                    RegisterName(toName(i, j), button);
                    button.Click += new RoutedEventHandler(click);
                    Grid.SetRow(button, i);
                    Grid.SetColumn(button, j);
                    board.Children.Add(button);
                }

            humanChessType = 'B'; //用户执黑先行
            init(humanChessType); //初始化
            gameOver = false;
        }

        private void clickUndoStep(object sender, RoutedEventArgs e)
        {           
            if (undoStep())
            {              
                int[] lastStepsArray = new int[4];
                getLastSteps(lastStepsArray);
                for (int i = 0; i < 2; i++)
                {
                    int row = lastStepsArray[i * 2];
                    int col = lastStepsArray[i * 2 + 1];
                    ((Button)board.FindName(toName(row, col))).Content = " ";
                }
            }
        }

        private void clickRedoStep(object sender, RoutedEventArgs e)
        {
            if (redoStep())
            {
                int[] lastStepsArray = new int[4];
                getLastSteps(lastStepsArray);
                for (int i = 0; i < 2; i++)
                {
                    int row = lastStepsArray[i * 2];
                    int col = lastStepsArray[i * 2 + 1];
                    if (i == 0)             
                        ((Button)board.FindName(toName(row, col))).Content = humanChessType == 'B' ? "●" : "○";
                    else
                        ((Button)board.FindName(toName(row, col))).Content = humanChessType == 'B' ? "○" : "●";
                }
            }
        }

        private void click(object sender, RoutedEventArgs e)
        {
            if (gameOver) return;
            //根据按钮Name得到按钮的(row, col)坐标
            var name = ((Button)sender).Name.ToCharArray();           
            int row = charDigitToInt(name[1], name[2]);
            int col = charDigitToInt(name[3], name[4]);    
            if (isValid(row, col))
            {              
                human(row, col);

                ((Button)sender).Content = humanChessType == 'B' ? "●" : "○";
                App.DoEvents(); //更新UI
                if (isGameOver(row, col))
                {
                    gameOver = true;
                    MessageBox.Show("You Won!");
                    return;
                }               
                computer(); //电脑落子         
                int computerRow = getComputerRow();
                int computerCol = getComputerCol();
                //找到电脑落子所在的按钮
                if (board.FindName(toName(computerRow, computerCol)) != null)
                {
                    ((Button)board.FindName(toName(computerRow, computerCol))).Content = humanChessType == 'B' ? "○" : "●";
                }
                if (isGameOver(computerRow, computerCol))
                {
                    gameOver = true;
                    MessageBox.Show("Computer Won!");
                }
                updateTurnAfterComputer();      
            }
        }

        private String toName(int row, int col)
        {
            String rowName = (row < 10 ? ("0" + row) : ("" + row));
            String colName = (col < 10 ? ("0" + col) : ("" + col));
            return "x" + rowName + colName;
        }

        private int charDigitToInt(char a, char b)
        {
            return (int)b - (int)'0' + ((int)a - (int)'0') * 10;
        }    
    }
}
