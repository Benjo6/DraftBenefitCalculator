namespace DraftBenefitCalculator.Client.Model;

public class DraftRequest
{
    public double Speed { get; set; }
    public double Distance { get; set; }
    public double Gradient { get; set; }
    public double Wind { get; set; }
    public double Angle { get; set; }
    public double Temp { get; set; }
    public double Pressure { get; set; }
    public double Humidity { get; set; }
    public double Area { get; set; }
    public double Drag { get; set; }
}

