#import "MTDirectionsRequest.h"
#import "MTDirectionsRequestMapQuest.h"
#import "MTDirectionsRequestGoogle.h"
#import "MTDirectionsParser.h"
#import "MTDirectionsAPI.h"

@interface MTDirectionsRequest ()

@property (nonatomic, strong) MTHTTPFetcher *fetcher;
@property (nonatomic, copy) NSString *fetcherAddress;
@property (nonatomic, assign) Class parserClass;

@end

@implementation MTDirectionsRequest

@synthesize fromCoordinate = fromCoordinate_;
@synthesize toCoordinate = toCoordinate_;
@synthesize completion = completion_;
@synthesize routeType = routeType_;
@synthesize fetcher = fetcher_;
@synthesize parserClass = parserClass_;

////////////////////////////////////////////////////////////////////////
#pragma mark - Lifecycle
////////////////////////////////////////////////////////////////////////

+ (id)requestFrom:(CLLocationCoordinate2D)fromCoordinate
               to:(CLLocationCoordinate2D)toCoordinate
       completion:(mt_direction_block)completion {
    return [self requestFrom:fromCoordinate
                          to:toCoordinate
                   routeType:kMTDefaultDirectionsRouteType
                  completion:completion];
}

+ (id)requestFrom:(CLLocationCoordinate2D)fromCoordinate
               to:(CLLocationCoordinate2D)toCoordinate
        routeType:(MTDirectionsRouteType)routeType
       completion:(mt_direction_block)completion {
    MTDirectionsRequest *request = nil;
    
    switch (MTDirectionsGetActiveAPI()) {
        case MTDirectionsAPIGoogle:
            request = [[MTDirectionsRequestGoogle alloc] initFrom:fromCoordinate
                                                               to:toCoordinate
                                                        routeType:routeType
                                                       completion:completion];
            break;
            
        case MTDirectionsAPIMapQuest:
        default:
            request = [[MTDirectionsRequestMapQuest alloc] initFrom:fromCoordinate
                                                                 to:toCoordinate
                                                          routeType:routeType
                                                         completion:completion];
            break;
            
    }
    
    return request;
}

- (id)initFrom:(CLLocationCoordinate2D)fromCoordinate
            to:(CLLocationCoordinate2D)toCoordinate
     routeType:(MTDirectionsRouteType)routeType
    completion:(mt_direction_block)completion {
    if ((self = [super init])) {
        fromCoordinate_ = fromCoordinate;
        toCoordinate_ = toCoordinate;
        completion_ = completion;
        routeType_ = routeType;
    }
    
    return self;
}

////////////////////////////////////////////////////////////////////////
#pragma mark - MTDirectionRequest
////////////////////////////////////////////////////////////////////////

- (void)setFetcherAddress:(NSString *)fetcherAddress {
    self.fetcher = [[MTHTTPFetcher alloc] initWithURLString:fetcherAddress
                                                   receiver:self
                                                     action:@selector(requestFinished:)];
}

- (NSString *)fetcherAddress {
    return self.fetcher.urlRequest.URL.absoluteString;
}

- (void)start {
    [self.fetcher start];
}

- (void)cancel {
    [self.fetcher cancel];
}

- (void)requestFinished:(MTHTTPFetcher *)fetcher {
    NSAssert([self.parserClass isSubclassOfClass:[MTDirectionsParser class]], @"Parser class must be subclass of MTDirectionsParser.");
    
    MTDirectionsParser *parser = [[self.parserClass alloc] initWithFromCoordinate:self.fromCoordinate
                                                                     toCoordinate:self.toCoordinate
                                                                        routeType:self.routeType
                                                                             data:fetcher.data];
    
    [parser parseWithCompletion:self.completion];
}

@end
