
class Robot
{
    private Communication communication;

    public Robot(int id)
    {
        communication = new Communication("_assets/communication/bot" + id + "/");
    }

    public void driveForward()
    {
        communication.writeMsg("drive_forward");
        communication.readMsg("done");
    }

    public void driveBackward()
    {
        communication.writeMsg("drive_backward");
        communication.readMsg("done");
    }

    public void rotateClockwise()
    {
        communication.writeMsg("rotate_cw");
        communication.readMsg("done");
    }

    public void rotateCounterClockwise()
    {
        communication.writeMsg("rotate_ccw");
        communication.readMsg("done");
    }
}