
using System;
using System.IO;
using System.Threading;

class Communication
{
    private string dir;

    public Communication(string name)
    {
        dir = name;
        System.IO.Directory.CreateDirectory(dir);
    }

    public void writeMsg(string msg)
    {
        Console.WriteLine("created " + msg);

        using (System.IO.File.Create(dir + msg))
        { }


        while (System.IO.File.Exists(dir + msg))
        {
            Thread.Sleep(10);
        }
        Console.WriteLine(msg + " was read");
    }

    public string[] readMsg(string msg)
    {
        Console.WriteLine("waiting for " + msg);
        while (!System.IO.File.Exists(dir + msg))
        {
            Thread.Sleep(10);
        }

        Console.WriteLine("found " + msg);

        string[] lines = System.IO.File.ReadAllLines(dir + msg);

        System.IO.File.Delete(dir + msg);

        return lines;
    }
}