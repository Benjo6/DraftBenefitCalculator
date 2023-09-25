using Blazor_MVVM.ViewModels;
using CommunityToolkit.Mvvm.ComponentModel;
using CommunityToolkit.Mvvm.Input;
using System.Net.Http.Json;
using DraftBenefitCalculator.Client.Model;

namespace DraftBenefitCalculator.Client.ViewModels;

public partial class SimpleCalculatorViewModel : ViewModelBase
{
    [ObservableProperty]
    private DraftRequest _request;
    [ObservableProperty]
    private double? _result;
    private HttpClient _http;

    public SimpleCalculatorViewModel(HttpClient http)
    {
        _http = http;
        Request = new DraftRequest();
        Loaded();

    }
    [RelayCommand]
    private async Task Calculate()
    {
        var request = new DraftRequest
        {
            Speed = Request.Speed,
            Distance = Request.Distance,
            Gradient = Request.Gradient,
            Wind = Request.Wind,
            Angle = Request.Angle,
            Temp = Request.Temp,
            Pressure = Request.Pressure,
            Humidity = Request.Humidity,
            Area = Request.Area,
            Drag = Request.Drag
        };
        var response = await _http.PostAsJsonAsync("api/draft", request);
        Result = await response.Content.ReadFromJsonAsync<double>();
    }
}
