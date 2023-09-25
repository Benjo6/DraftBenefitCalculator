using Blazor_MVVM.Components;
using DraftBenefitCalculator.Client;
using DraftBenefitCalculator.Client.Extensions;
using Microsoft.AspNetCore.Components.Web;
using Microsoft.AspNetCore.Components.WebAssembly.Hosting;

var builder = WebAssemblyHostBuilder.CreateDefault(args);
builder.RootComponents.Add<App>("#app");
builder.RootComponents.Add<HeadOutlet>("head::after");

builder.Services.AddScoped(sp => new HttpClient { BaseAddress = new Uri("http://localhost:8080/") });
builder.Services.AddScoped<IMvvmNavigationManager, MvvmNavigationManager>();
builder.Services.AddViewModels();


await builder.Build().RunAsync();
