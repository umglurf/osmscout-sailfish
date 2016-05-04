#ifndef DBTHREAD_H
#define DBTHREAD_H

/*
 OSMScout - a Qt backend for libosmscout and libosmscout-map
 Copyright (C) 2010  Tim Teulings

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <QtGui>
#include <QThread>
#include <QMetaType>
#include <QMutex>
#include <QTime>
#include <QTimer>

#include <osmscout/Database.h>
#include <osmscout/LocationService.h>
#include <osmscout/MapService.h>
#include <osmscout/RoutingService.h>
#include <osmscout/RoutePostprocessor.h>

#include <osmscout/MapPainterQt.h>

#include <osmscout/util/Breaker.h>

#include "Settings.h"
#include "TileCache.h"
#include "OsmTileDownloader.h"

struct RenderMapRequest
{
  double                  lon;
  double                  lat;
  double                  angle;
  osmscout::Magnification magnification;
  size_t                  width;
  size_t                  height;
};

Q_DECLARE_METATYPE(RenderMapRequest)

struct DatabaseLoadedResponse
{
    osmscout::GeoBox boundingBox;
};

Q_DECLARE_METATYPE(DatabaseLoadedResponse)

class QBreaker : public osmscout::Breaker
{
private:
  mutable QMutex mutex;
  bool           aborted;

public:
  QBreaker();

  bool Break();
  bool IsAborted() const;
  void Reset();
};

class DBThread : public QObject
{
  Q_OBJECT

signals:
  void InitialisationFinished(const DatabaseLoadedResponse& response);
  void TriggerInitialRendering();
  void TriggerDrawMap();
  void Redraw();
  void TileStatusChanged(const osmscout::TileRef& tile);
  void triggerTileRequest(uint32_t zoomLevel, uint32_t xtile, uint32_t ytile);

public slots:
  void ToggleDaylight();
  void ReloadStyle();
  void HandleInitialRenderingRequest();
  void HandleTileStatusChanged(const osmscout::TileRef& changedTile);
  void DrawTileMap(QPainter &p, const osmscout::GeoCoord box, uint32_t z, size_t width, size_t height, bool drawBackground);
  void Initialize();
  void Finalize();
  void tileRequest(uint32_t zoomLevel, uint32_t xtile, uint32_t ytile);
  void tileDownloaded(uint32_t zoomLevel, uint32_t x, uint32_t y, QImage image, QByteArray downloadedData);
  void tileDownloadFailed(uint32_t zoomLevel, uint32_t x, uint32_t y);  

private:
  QString                       databaseDirectory; 
  QString                       resourceDirectory;
  QString                       tileCacheDirectory;
  
  double                        dpi;

  mutable QMutex                mutex;
  
  TileCache                     tileCache;
  OsmTileDownloader             *tileDownloader;

  osmscout::DatabaseParameter   databaseParameter;
  osmscout::DatabaseRef         database;
  osmscout::LocationServiceRef  locationService;
  osmscout::MapServiceRef       mapService;
  osmscout::MapService::CallbackId callbackId;
  //osmscout::MercatorProjection  projection;
  osmscout::RouterParameter     routerParameter;
  osmscout::RoutingServiceRef   router;
  osmscout::RoutePostprocessor  routePostprocessor;

  QString                       stylesheetFilename;
  bool                          daylight;
  osmscout::StyleConfigRef      styleConfig;
  osmscout::MapPainterQt        *painter;
  QString                       iconDirectory;

  osmscout::BreakerRef          dataLoadingBreaker;

private:
  DBThread(QString databaseDirectory, QString resourceDirectory, QString tileCacheDirectory, double dpi = -1);
  virtual ~DBThread();

  bool AssureRouter(osmscout::Vehicle vehicle);

  void TileStateCallback(const osmscout::TileRef& changedTile);

public:
  bool isInitialized(); 
  
  const DatabaseLoadedResponse loadedResponse() const;
  
  const double GetDpi() const;
  
  void CancelCurrentDataLoading();

  bool RenderMap(QPainter& painter,
                 const RenderMapRequest& request);
  
  osmscout::TypeConfigRef GetTypeConfig() const;

  bool GetNodeByOffset(osmscout::FileOffset offset,
                       osmscout::NodeRef& node) const;
  bool GetAreaByOffset(osmscout::FileOffset offset,
                       osmscout::AreaRef& relation) const;
  bool GetWayByOffset(osmscout::FileOffset offset,
                      osmscout::WayRef& way) const;

  bool ResolveAdminRegionHierachie(const osmscout::AdminRegionRef& adminRegion,
                                   std::map<osmscout::FileOffset,osmscout::AdminRegionRef >& refs) const;

  bool SearchForLocations(const std::string& searchPattern,
                          size_t limit,
                          osmscout::LocationSearchResult& result) const;

  bool CalculateRoute(osmscout::Vehicle vehicle,
                      const osmscout::RoutingProfile& routingProfile,
                      const osmscout::ObjectFileRef& startObject,
                      size_t startNodeIndex,
                      const osmscout::ObjectFileRef targetObject,
                      size_t targetNodeIndex,
                      osmscout::RouteData& route);

  bool TransformRouteDataToRouteDescription(osmscout::Vehicle vehicle,
                                            const osmscout::RoutingProfile& routingProfile,
                                            const osmscout::RouteData& data,
                                            osmscout::RouteDescription& description,
                                            const std::string& start,
                                            const std::string& target);
  bool TransformRouteDataToWay(osmscout::Vehicle vehicle,
                               const osmscout::RouteData& data,
                               osmscout::Way& way);

  bool GetClosestRoutableNode(const osmscout::ObjectFileRef& refObject,
                              const osmscout::Vehicle& vehicle,
                              double radius,
                              osmscout::ObjectFileRef& object,
                              size_t& nodeIndex);

  void ClearRoute();
  void AddRoute(const osmscout::Way& way);

  static bool InitializeInstance(QString databaseDirectory, QString resourceDirectory, QString tileCacheDirectory, double dpi = -1);
  static DBThread* GetInstance();
  static void FreeInstance();
};

#endif
