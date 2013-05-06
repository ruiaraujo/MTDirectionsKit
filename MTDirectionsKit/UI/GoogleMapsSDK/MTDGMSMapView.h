//
//  MTDMapView.h
//  MTDirectionsKit
//
//  Created by Matthias Tretter
//  Copyright (c) 2012 Matthias Tretter (@myell0w). All rights reserved.
//


#import "MTDMapViewProtocol.h"
#import "MTDDirectionsRouteType.h"
#import "MTDDirectionsDisplayType.h"
#import "MTDDirectionsDefines.h"
#import "MTDDirectionsRequestOption.h"
#import "MTDWeak.h"
#import <GoogleMaps/GoogleMaps.h>


@class MTDDistance;
@class MTDWaypoint;
@class MTDDirectionsOverlay;
@class MTDGMSDirectionsOverlayView;
@class MTDRoute;
@protocol MTDDirectionsDelegate;


/**
 An MTDMapView instance provides functionality to show directions directly on top of the MapView inside your App.
 MTDMapView is a subclass of MKMapView and therefore uses Apple's standard way of displaying map data, which we all
 know and love. Up until iOS 5 this means that Google Maps is used as a backend for the map data, starting with iOS 6
 Apple Maps are used.
 
 Sample usage:
 
 MTDMapView *_mapView = [[MTDMapView alloc] initWithFrame:self.view.bounds];
 _mapView.directionsDelegate = self;
 
 [_mapView loadDirectionsFrom:CLLocationCoordinate2DMake(51.38713, -1.0316)
                           to:CLLocationCoordinate2DMake(51.4554, -0.9742)
                    routeType:MTDDirectionsRouteTypeFastestDriving
         zoomToShowDirections:YES];
 
 */
@interface MTDGMSMapView : GMSMapView <MTDMapView>

/******************************************
 @name Delegate
 ******************************************/

/** The receiver's directionsDelegate */
@property (nonatomic, mtd_weak) id<MTDDirectionsDelegate> directionsDelegate;

/******************************************
 @name Directions
 ******************************************/

/** 
 The current display type of the directions overlay. You can change the way the directions
 are shown on top of your instance of MTDMapView by changing the property. A change results
 in a re-draw of the overlay.
 
 Currently there are the following types supported:
 
 - MTDDirectionsDisplayTypeNone: don't display anything
 - MTDDirectionsDisplayTypeOverview: displays a polyline with all Waypoints of the route
 */
@property (nonatomic, assign) MTDDirectionsDisplayType directionsDisplayType;

/** the starting coordinate of the directions of the currently displayed overlay */
@property (nonatomic, readonly) CLLocationCoordinate2D fromCoordinate;
/** the end coordinate of the directions of the currently displayed overlay */
@property (nonatomic, readonly) CLLocationCoordinate2D toCoordinate;

/** the total distance of the directions of the currenty displayed overlay in meters */
@property (nonatomic, readonly) CLLocationDistance distanceInMeter;
/** the total estimated time of the directions */
@property (nonatomic, readonly) NSTimeInterval timeInSeconds;

/** 
 The type of travelling used to compute the directions of the currently displayed overlay 
 
 The following types of travelling are supported:
 
 - MTDDirectionsRouteTypeFastestDriving
 - MTDDirectionsRouteTypeShortestDriving
 - MTDDirectionsRouteTypePedestrian
 - MTDDirectionsRouteTypePedestrianIncludingPublicTransport
 - MTDDirectionsRouteTypeBicycle
 */
@property (nonatomic, readonly) MTDDirectionsRouteType routeType;

/**
 The padding that will be used when zooming the map to the displayed directions.
 If not specified, a default padding of 125.f will be used.
 */
@property (nonatomic, assign) CGFloat directionsEdgePadding;


/**
 Returns the corresponding directions overlay view for the given route
 
 @param route the route which overlay view we are interested in
 @return the overlay view of the given route or nil
 */
- (MTDGMSDirectionsOverlayView *)directionsOverlayViewForRoute:(MTDRoute *)route;

/**
 Returns the minimum distance in points between the givon coordinate and the currently active route.
 You can use this method to determine if the user has moved away a specified value from the desired
 route and reload the directions in that case.

 @param coordinate a coordinate on the map
 @return in case there currently is an active route the minimum distance between the coordinate to any line segment of the route.
 In case there is no active route, FLT_MAX is returned
 */
- (CGFloat)distanceBetweenActiveRouteAndCoordinate:(CLLocationCoordinate2D)coordinate;

@end