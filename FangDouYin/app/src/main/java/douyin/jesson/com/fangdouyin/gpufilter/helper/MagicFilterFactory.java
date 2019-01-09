package douyin.jesson.com.fangdouyin.gpufilter.helper;


import douyin.jesson.com.fangdouyin.gpufilter.basefilter.GPUImageFilter;
import douyin.jesson.com.fangdouyin.gpufilter.filter.MagicAntiqueFilter;
import douyin.jesson.com.fangdouyin.gpufilter.filter.MagicBrannanFilter;
import douyin.jesson.com.fangdouyin.gpufilter.filter.MagicCoolFilter;
import douyin.jesson.com.fangdouyin.gpufilter.filter.MagicFreudFilter;
import douyin.jesson.com.fangdouyin.gpufilter.filter.MagicHefeFilter;
import douyin.jesson.com.fangdouyin.gpufilter.filter.MagicHudsonFilter;
import douyin.jesson.com.fangdouyin.gpufilter.filter.MagicInkwellFilter;
import douyin.jesson.com.fangdouyin.gpufilter.filter.MagicN1977Filter;
import douyin.jesson.com.fangdouyin.gpufilter.filter.MagicNashvilleFilter;

public class MagicFilterFactory {

    private static MagicFilterType filterType = MagicFilterType.NONE;

    public static GPUImageFilter initFilters(MagicFilterType type) {
        if (type == null) {
            return null;
        }
        filterType = type;
        switch (type) {
            case ANTIQUE:
                return new MagicAntiqueFilter();
            case BRANNAN:
                return new MagicBrannanFilter();
            case FREUD:
                return new MagicFreudFilter();
            case HEFE:
                return new MagicHefeFilter();
            case HUDSON:
                return new MagicHudsonFilter();
            case INKWELL:
                return new MagicInkwellFilter();
            case N1977:
                return new MagicN1977Filter();
            case NASHVILLE:
                return new MagicNashvilleFilter();
            case COOL:
                return new MagicCoolFilter();
            case WARM:
                return new MagicWarmFilter();
            default:
                return null;
        }
    }

    public MagicFilterType getCurrentFilterType() {
        return filterType;
    }

    private static class MagicWarmFilter extends GPUImageFilter {
    }
}
