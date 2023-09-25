using DraftBenefitCalculator.Client.ViewModels;

namespace DraftBenefitCalculator.Client.Extensions;

public static class ServicesExtension
{
    public static IServiceCollection AddViewModels(this IServiceCollection services)
    {
        services.AddTransient<PictureCalculatorViewModel>();
        services.AddTransient<ScenarioCalculatorViewModel>();
        services.AddTransient<SimpleCalculatorViewModel>();

        return services;
    }
}
