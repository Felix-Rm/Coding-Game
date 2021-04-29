class Robot0
{
    public static void Main()
    {
        Robot robot = new Robot(0);

        robot.rotateClockwise();
        robot.driveBackward();
        robot.rotateClockwise();
        for (int i = 0; i < 4; i++)
            robot.driveBackward();

        robot.rotateCounterClockwise();
        robot.rotateCounterClockwise();

        robot.driveForward();
        robot.driveForward();
        robot.driveForward();
    }
}
