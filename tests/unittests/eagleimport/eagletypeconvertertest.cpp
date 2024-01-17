/*
 * LibrePCB - Professional EDA for everyone!
 * Copyright (C) 2013 LibrePCB Developers, see AUTHORS.md for contributors.
 * https://librepcb.org/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*******************************************************************************
 *  Includes
 ******************************************************************************/
#include <gtest/gtest.h>
#include <librepcb/core/library/pkg/footprintpad.h>
#include <librepcb/core/library/pkg/packagepad.h>
#include <librepcb/core/library/sym/symbolpin.h>
#include <librepcb/core/types/layer.h>
#include <librepcb/core/types/point.h>
#include <librepcb/eagleimport/eagletypeconverter.h>
#include <parseagle/common/domelement.h>
#include <parseagle/library.h>

#include <QtCore>

/*******************************************************************************
 *  Namespace
 ******************************************************************************/
namespace librepcb {
namespace eagleimport {
namespace tests {

using C = EagleTypeConverter;

/*******************************************************************************
 *  Test Class
 ******************************************************************************/

class EagleTypeConverterTest : public ::testing::Test {
protected:
  static parseagle::DomElement dom(const QString& str) {
    QDomDocument doc;
    doc.setContent(str);
    return parseagle::DomElement(doc.documentElement());
  }
};

/*******************************************************************************
 *  Test Methods
 ******************************************************************************/

TEST_F(EagleTypeConverterTest, testConvertElementName) {
  EXPECT_EQ("Valid Name", C::convertElementName("Valid Name")->toStdString());
  EXPECT_EQ("X", C::convertElementName(" \nX ")->toStdString());
  EXPECT_EQ("Unnamed", C::convertElementName("\n")->toStdString());
}

TEST_F(EagleTypeConverterTest, testConvertElementDescription) {
  EXPECT_EQ("", C::convertElementDescription("").toStdString());
  EXPECT_EQ("Text", C::convertElementDescription(" Text ").toStdString());
  EXPECT_EQ("X\nY", C::convertElementDescription("X\nY").toStdString());
  EXPECT_EQ("X\nY",
            C::convertElementDescription("<b>X</b><br/>Y").toStdString());
  EXPECT_EQ("X\nY",
            C::convertElementDescription("<b>X</b>\n<br/>Y").toStdString());
}

TEST_F(EagleTypeConverterTest, testConvertComponentName) {
  EXPECT_EQ("Valid Name", C::convertComponentName("Valid Name")->toStdString());
  EXPECT_EQ("X", C::convertComponentName(" \nX ")->toStdString());
  EXPECT_EQ("Foo - Bar", C::convertComponentName("Foo - Bar-")->toStdString());
  EXPECT_EQ("Foo _ Bar", C::convertComponentName("Foo _ Bar_")->toStdString());
  EXPECT_EQ("-", C::convertComponentName("-")->toStdString());
  EXPECT_EQ("Unnamed", C::convertComponentName("\n")->toStdString());
}

TEST_F(EagleTypeConverterTest, testConvertDeviceName) {
  EXPECT_EQ("Valid Name",
            C::convertDeviceName("Valid Name", "")->toStdString());
  EXPECT_EQ("Valid Name-Foo",
            C::convertDeviceName("Valid Name", "Foo")->toStdString());
  EXPECT_EQ("Valid Name-Foo",
            C::convertDeviceName("Valid Name-", "Foo")->toStdString());
  EXPECT_EQ("Valid Name_Foo",
            C::convertDeviceName("Valid Name_", "Foo")->toStdString());
  EXPECT_EQ("Valid Name-Foo",
            C::convertDeviceName("Valid Name-", "Foo")->toStdString());
  EXPECT_EQ("Valid Name_Foo",
            C::convertDeviceName("Valid Name_", "Foo")->toStdString());
  EXPECT_EQ("Valid Name-Foo",
            C::convertDeviceName("Valid Name", "-Foo")->toStdString());
  EXPECT_EQ("Valid Name_Foo",
            C::convertDeviceName("Valid Name", "_Foo")->toStdString());
  EXPECT_EQ("X", C::convertDeviceName(" \nX ", "")->toStdString());
  EXPECT_EQ("Unnamed", C::convertDeviceName("\n", "")->toStdString());
  EXPECT_EQ("Unnamed", C::convertDeviceName("", "")->toStdString());
}

TEST_F(EagleTypeConverterTest, testConvertGateName) {
  EXPECT_EQ("", C::convertGateName("")->toStdString());
  EXPECT_EQ("", C::convertGateName("G$42")->toStdString());
  EXPECT_EQ("1", C::convertGateName("-1")->toStdString());
  EXPECT_EQ("Foo_Bar", C::convertGateName(" Foo Bar ")->toStdString());
}

TEST_F(EagleTypeConverterTest, testConvertPinOrPadName) {
  EXPECT_EQ("Unnamed", C::convertPinOrPadName(" ")->toStdString());
  EXPECT_EQ("42", C::convertPinOrPadName("P$42")->toStdString());
  EXPECT_EQ("3", C::convertPinOrPadName("3")->toStdString());
  EXPECT_EQ("Foo_Bar", C::convertPinOrPadName(" Foo Bar ")->toStdString());
}

TEST_F(EagleTypeConverterTest, testTryConvertSchematicLayer) {
  EXPECT_EQ(nullptr, C::tryConvertSchematicLayer(1));  // tCu
  EXPECT_EQ(&Layer::symbolOutlines(), C::tryConvertSchematicLayer(94));  // sym
  EXPECT_EQ(nullptr, C::tryConvertSchematicLayer(999));  // non existent
}

TEST_F(EagleTypeConverterTest, testTryConvertBoardLayer) {
  EXPECT_EQ(&Layer::topCopper(), C::tryConvertBoardLayer(1));  // tCu
  EXPECT_EQ(Layer::innerCopper(2), C::tryConvertBoardLayer(3));  // inner 2
  EXPECT_EQ(&Layer::botCopper(), C::tryConvertBoardLayer(16));  // bCu
  EXPECT_EQ(nullptr, C::tryConvertBoardLayer(94));  // symbols
  EXPECT_EQ(nullptr, C::tryConvertBoardLayer(999));  // non existent
}

TEST_F(EagleTypeConverterTest, testConvertLength) {
  EXPECT_EQ(Length(0), C::convertLength(0));
  EXPECT_EQ(Length(-1234567), C::convertLength(-1.234567));
  EXPECT_EQ(Length(1234567), C::convertLength(1.234567));
}

TEST_F(EagleTypeConverterTest, testConvertPoint) {
  EXPECT_EQ(Point(0, 0), C::convertPoint(parseagle::Point{0, 0}));
  EXPECT_EQ(Point(-1234567, 1234567),
            C::convertPoint(parseagle::Point{-1.234567, 1.234567}));
}

TEST_F(EagleTypeConverterTest, testConvertAngle) {
  EXPECT_EQ(Angle(0), C::convertAngle(0));
  EXPECT_EQ(Angle(-1234567), C::convertAngle(-1.234567));
  EXPECT_EQ(Angle(1234567), C::convertAngle(1.234567));
}

TEST_F(EagleTypeConverterTest, testConvertVertex) {
  EXPECT_EQ(
      Vertex(Point(0, 0), Angle(0)),
      C::convertVertex(parseagle::Vertex(dom("<vertex x=\"0\" y=\"0\"/>"))));
  EXPECT_EQ(Vertex(Point(-6350000, 2540000), Angle(90000000)),
            C::convertVertex(parseagle::Vertex(
                dom("<vertex x=\"-6.35\" y=\"2.54\" curve=\"90\"/>"))));
}

TEST_F(EagleTypeConverterTest, testConvertVertices) {
  QList<parseagle::Vertex> vertices{
      parseagle::Vertex(dom("<vertex x=\"-45.72\" y=\"-5.08\" curve=\"45\"/>")),
      parseagle::Vertex(dom("<vertex x=\"-35.56\" y=\"-5.08\"/>")),
      parseagle::Vertex(dom("<vertex x=\"-38.1\" y=\"-12.7\"/>")),
  };
  Path expected({
      Vertex(Point(-45720000, -5080000), Angle(45000000)),
      Vertex(Point(-35560000, -5080000), Angle(0)),
      Vertex(Point(-38100000, -12700000), Angle(0)),
      Vertex(Point(-45720000, -5080000), Angle(0)),
  });
  EXPECT_EQ(expected, C::convertVertices(vertices, true));
}

TEST_F(EagleTypeConverterTest, testConvertAndJoinWires) {
  QStringList errors;
  QList<parseagle::Wire> wires{
      // clang-format off
      parseagle::Wire(dom("<wire x1=\"1\" y1=\"2\" x2=\"3\" y2=\"4\" width=\"0.254\" layer=\"1\"/>")),
      parseagle::Wire(dom("<wire x1=\"3\" y1=\"4\" x2=\"5\" y2=\"6\" width=\"0.254\" layer=\"1\"/>")),
      parseagle::Wire(dom("<wire x1=\"5\" y1=\"6\" x2=\"7\" y2=\"8\" width=\"0.567\" layer=\"1\"/>")),
      parseagle::Wire(dom("<wire x1=\"7\" y1=\"8\" x2=\"9\" y2=\"9\" width=\"0.567\" layer=\"2\"/>")),
      parseagle::Wire(dom("<wire x1=\"7\" y1=\"8\" x2=\"9\" y2=\"9\" width=\"-1\" layer=\"2\"/>")),
      // clang-format on
  };
  auto out = C::convertAndJoinWires(wires, true, &errors);
  ASSERT_EQ(3, out.count());
  EXPECT_EQ(1, errors.count());

  EXPECT_EQ(1, out.at(0).layerId);
  EXPECT_EQ(UnsignedLength(254000), out.at(0).lineWidth);
  EXPECT_EQ(false, out.at(0).filled);
  EXPECT_EQ(false, out.at(0).grabArea);
  EXPECT_EQ(Path({
                Vertex(Point(1000000, 2000000), Angle(0)),
                Vertex(Point(3000000, 4000000), Angle(0)),
                Vertex(Point(5000000, 6000000), Angle(0)),
            }),
            out.at(0).path);

  EXPECT_EQ(1, out.at(1).layerId);
  EXPECT_EQ(UnsignedLength(567000), out.at(1).lineWidth);
  EXPECT_EQ(false, out.at(1).filled);
  EXPECT_EQ(false, out.at(1).grabArea);
  EXPECT_EQ(Path({
                Vertex(Point(5000000, 6000000), Angle(0)),
                Vertex(Point(7000000, 8000000), Angle(0)),
            }),
            out.at(1).path);

  EXPECT_EQ(2, out.at(2).layerId);
  EXPECT_EQ(UnsignedLength(567000), out.at(2).lineWidth);
  EXPECT_EQ(false, out.at(2).filled);
  EXPECT_EQ(false, out.at(2).grabArea);
  EXPECT_EQ(Path({
                Vertex(Point(7000000, 8000000), Angle(0)),
                Vertex(Point(9000000, 9000000), Angle(0)),
            }),
            out.at(2).path);
}

TEST_F(EagleTypeConverterTest, testConvertRectangle) {
  QString xml = "<rectangle x1=\"1\" y1=\"2\" x2=\"4\" y2=\"3\" layer=\"1\"/>";
  auto out = C::convertRectangle(parseagle::Rectangle(dom(xml)), true);
  EXPECT_EQ(1, out.layerId);
  EXPECT_EQ(UnsignedLength(0), out.lineWidth);
  EXPECT_EQ(true, out.filled);  // EAGLE rectangles are always filled.
  EXPECT_EQ(true, out.grabArea);  // Passed to function under test.
  EXPECT_EQ(Path({
                Vertex(Point(1000000, 2000000), Angle(0)),
                Vertex(Point(4000000, 2000000), Angle(0)),
                Vertex(Point(4000000, 3000000), Angle(0)),
                Vertex(Point(1000000, 3000000), Angle(0)),
                Vertex(Point(1000000, 2000000), Angle(0)),
            }),
            out.path);
  EXPECT_EQ(tl::nullopt, out.circle);
}

TEST_F(EagleTypeConverterTest, testConvertRectangleRotated) {
  QString xml =
      "<rectangle x1=\"1\" y1=\"2\" x2=\"4\" y2=\"3\" layer=\"1\" "
      "rot=\"R90\"/>";
  auto out = C::convertRectangle(parseagle::Rectangle(dom(xml)), false);
  EXPECT_EQ(1, out.layerId);
  EXPECT_EQ(UnsignedLength(0), out.lineWidth);
  EXPECT_EQ(true, out.filled);  // EAGLE rectangles are always filled.
  EXPECT_EQ(false, out.grabArea);  // Passed to function under test.
  EXPECT_EQ(Path({
                Vertex(Point(3000000, 1000000), Angle(0)),
                Vertex(Point(3000000, 4000000), Angle(0)),
                Vertex(Point(2000000, 4000000), Angle(0)),
                Vertex(Point(2000000, 1000000), Angle(0)),
                Vertex(Point(3000000, 1000000), Angle(0)),
            }),
            out.path);
  EXPECT_EQ(tl::nullopt, out.circle);
}

TEST_F(EagleTypeConverterTest, testConvertPolygon) {
  QString xml =
      "<polygon width=\"2.54\" layer=\"1\">"
      "<vertex x=\"1\" y=\"2\" curve=\"45\"/>"
      "<vertex x=\"3\" y=\"4\"/>"
      "</polygon>";
  auto out = C::convertPolygon(parseagle::Polygon(dom(xml)), false);
  EXPECT_EQ(1, out.layerId);
  EXPECT_EQ(UnsignedLength(2540000), out.lineWidth);
  EXPECT_EQ(true, out.filled);  // EAGLE polygons are always filled.
  EXPECT_EQ(false, out.grabArea);  // Passed to function under test.
  EXPECT_EQ(Path({
                Vertex(Point(1000000, 2000000), Angle(45000000)),
                Vertex(Point(3000000, 4000000), Angle(0)),
                Vertex(Point(1000000, 2000000), Angle(0)),
            }),
            out.path);
  EXPECT_EQ(tl::nullopt, out.circle);
}

TEST_F(EagleTypeConverterTest, testConvertCircle) {
  QString xml =
      "<circle x=\"1\" y=\"2\" radius=\"3.5\" width=\"0.254\" layer=\"1\"/>";
  auto out = C::convertCircle(parseagle::Circle(dom(xml)), true);
  EXPECT_EQ(1, out.layerId);
  EXPECT_EQ(UnsignedLength(254000), out.lineWidth);
  EXPECT_EQ(false, out.filled);  // Not filled if line width != 0.
  EXPECT_EQ(true, out.grabArea);  // Passed to function under test.
  EXPECT_EQ(Path({
                Vertex(Point(4500000, 2000000), -Angle::deg180()),
                Vertex(Point(-2500000, 2000000), -Angle::deg180()),
                Vertex(Point(4500000, 2000000), Angle(0)),
            }),
            out.path);
  ASSERT_TRUE(out.circle.has_value());
  EXPECT_EQ(Point(1000000, 2000000), out.circle->first);
  EXPECT_EQ(PositiveLength(7000000), out.circle->second);
}

TEST_F(EagleTypeConverterTest, testConvertCircleFilled) {
  QString xml =
      "<circle x=\"1\" y=\"2\" radius=\"3.5\" width=\"0\" layer=\"1\"/>";
  auto out = C::convertCircle(parseagle::Circle(dom(xml)), false);
  EXPECT_EQ(1, out.layerId);
  EXPECT_EQ(UnsignedLength(0), out.lineWidth);
  EXPECT_EQ(true, out.filled);  // Filled if line width == 0.
  EXPECT_EQ(false, out.grabArea);  // Passed to function under test.
  EXPECT_EQ(Path({
                Vertex(Point(4500000, 2000000), -Angle::deg180()),
                Vertex(Point(-2500000, 2000000), -Angle::deg180()),
                Vertex(Point(4500000, 2000000), Angle(0)),
            }),
            out.path);
  ASSERT_TRUE(out.circle.has_value());
  EXPECT_EQ(Point(1000000, 2000000), out.circle->first);
  EXPECT_EQ(PositiveLength(7000000), out.circle->second);
}

TEST_F(EagleTypeConverterTest, testConvertHole) {
  QString xml = "<hole x=\"1\" y=\"2\" drill=\"3.5\"/>";
  auto out = C::convertHole(parseagle::Hole(dom(xml)));
  EXPECT_EQ(PositiveLength(3500000), out->getDiameter());
  EXPECT_EQ(1, out->getPath()->getVertices().count());
  EXPECT_EQ(Point(1000000, 2000000),
            out->getPath()->getVertices().first().getPos());
}

TEST_F(EagleTypeConverterTest, testConvertTextValue) {
  EXPECT_EQ("", C::convertTextValue("").toStdString());
  EXPECT_EQ("{{NAME}}", C::convertTextValue(">NAME").toStdString());
  EXPECT_EQ("{{VALUE}}", C::convertTextValue(">VALUE").toStdString());
  EXPECT_EQ("Some Text", C::convertTextValue("Some Text").toStdString());
}

TEST_F(EagleTypeConverterTest, testTryConvertSchematicText) {
  QString xml = "<text x=\"1\" y=\"2\" size=\"3\" layer=\"94\">foo\nbar</text>";
  auto out = C::tryConvertSchematicText(parseagle::Text(dom(xml)));
  ASSERT_TRUE(out);
  EXPECT_EQ(Layer::symbolOutlines().getId().toStdString(),
            out->getLayer().getId().toStdString());
  EXPECT_EQ(Point(1000000, 2000000), out->getPosition());
  EXPECT_EQ(Angle(0), out->getRotation());
  EXPECT_EQ(PositiveLength(2500000), out->getHeight());  // Default (hardcoded).
  EXPECT_EQ(Alignment(HAlign::left(), VAlign::bottom()),
            out->getAlign());  // Default (hardcoded).
  EXPECT_EQ("foo\nbar", out->getText().toStdString());
}

TEST_F(EagleTypeConverterTest, testTryConvertBoardText) {
  QString xml = "<text x=\"1\" y=\"2\" size=\"3\" layer=\"1\">&gt;NAME</text>";
  auto out = C::tryConvertBoardText(parseagle::Text(dom(xml)));
  ASSERT_TRUE(out);
  EXPECT_EQ(Layer::topCopper().getId().toStdString(),
            out->getLayer().getId().toStdString());
  EXPECT_EQ(Point(1000000, 2000000), out->getPosition());
  EXPECT_EQ(Angle(0), out->getRotation());
  EXPECT_EQ(PositiveLength(1000000), out->getHeight());  // Default (hardcoded).
  EXPECT_EQ(UnsignedLength(200000),
            out->getStrokeWidth());  // Default (hardcoded).
  EXPECT_EQ(StrokeTextSpacing(),
            out->getLetterSpacing());  // Default (hardcoded).
  EXPECT_EQ(StrokeTextSpacing(),
            out->getLineSpacing());  // Default (hardcoded).
  EXPECT_EQ(Alignment(HAlign::left(), VAlign::bottom()),
            out->getAlign());  // Default (hardcoded).
  EXPECT_EQ(false, out->getMirrored());  // Default (hardcoded).
  EXPECT_EQ(true, out->getAutoRotate());  // Default (hardcoded).
  EXPECT_EQ("{{NAME}}", out->getText().toStdString());
}

TEST_F(EagleTypeConverterTest, testConvertSymbolPin) {
  QString xml = "<pin name=\"P$1\" x=\"1\" y=\"2\" length=\"point\"/>";
  auto out = C::convertSymbolPin(parseagle::Pin(dom(xml)));
  EXPECT_EQ("1", out->getName()->toStdString());
  EXPECT_EQ(Point(1000000, 2000000), out->getPosition());
  EXPECT_EQ(UnsignedLength(0), out->getLength());
  EXPECT_EQ(Angle(0), out->getRotation());
}

TEST_F(EagleTypeConverterTest, testConvertSymbolPinRotated) {
  QString xml =
      "<pin name=\"P$1\" x=\"1\" y=\"2\" length=\"middle\" rot=\"R90\"/>";
  auto out = C::convertSymbolPin(parseagle::Pin(dom(xml)));
  EXPECT_EQ("1", out->getName()->toStdString());
  EXPECT_EQ(Point(1000000, 2000000), out->getPosition());
  EXPECT_EQ(UnsignedLength(5080000), out->getLength());
  EXPECT_EQ(Angle(90000000), out->getRotation());
}

TEST_F(EagleTypeConverterTest, testConvertThtPad) {
  QString xml =
      "<pad name=\"P$1\" x=\"1\" y=\"2\" drill=\"1.5\" shape=\"square\"/>";
  auto out = C::convertThtPad(parseagle::ThtPad(dom(xml)));
  EXPECT_EQ("1", out.first->getName()->toStdString());
  EXPECT_EQ(out.first->getUuid(), out.second->getPackagePadUuid());
  EXPECT_EQ(Point(1000000, 2000000), out.second->getPosition());
  EXPECT_EQ(Angle(0), out.second->getRotation());
  EXPECT_EQ(FootprintPad::Shape::RoundedRect, out.second->getShape());
  EXPECT_EQ(PositiveLength(2250000), out.second->getWidth());  // 1.5*drill
  EXPECT_EQ(PositiveLength(2250000), out.second->getHeight());  // 1.5*drill
  EXPECT_EQ(FootprintPad::ComponentSide::Top, out.second->getComponentSide());
  ASSERT_EQ(1, out.second->getHoles().count());
  EXPECT_EQ(PositiveLength(1500000),
            out.second->getHoles().first()->getDiameter());
}

TEST_F(EagleTypeConverterTest, testConvertThtPadRotated) {
  QString xml =
      "<pad name=\"P$1\" x=\"1\" y=\"2\" drill=\"1.5\" diameter=\"2.54\" "
      "shape=\"octagon\" rot=\"R90\"/>";
  auto out = C::convertThtPad(parseagle::ThtPad(dom(xml)));
  EXPECT_EQ("1", out.first->getName()->toStdString());
  EXPECT_EQ(out.first->getUuid(), out.second->getPackagePadUuid());
  EXPECT_EQ(Point(1000000, 2000000), out.second->getPosition());
  EXPECT_EQ(Angle(90000000), out.second->getRotation());
  EXPECT_EQ(FootprintPad::Shape::RoundedOctagon, out.second->getShape());
  EXPECT_EQ(PositiveLength(2540000), out.second->getWidth());
  EXPECT_EQ(PositiveLength(2540000), out.second->getHeight());
  EXPECT_EQ(FootprintPad::ComponentSide::Top, out.second->getComponentSide());
  ASSERT_EQ(1, out.second->getHoles().count());
  EXPECT_EQ(PositiveLength(1500000),
            out.second->getHoles().first()->getDiameter());
}

TEST_F(EagleTypeConverterTest, testConvertSmtPad) {
  QString xml =
      "<smd name=\"P$1\" x=\"1\" y=\"2\" dx=\"3\" dy=\"4\" layer=\"1\"/>";
  auto out = C::convertSmtPad(parseagle::SmtPad(dom(xml)));
  EXPECT_EQ("1", out.first->getName()->toStdString());
  EXPECT_EQ(out.first->getUuid(), out.second->getPackagePadUuid());
  EXPECT_EQ(Point(1000000, 2000000), out.second->getPosition());
  EXPECT_EQ(Angle(0), out.second->getRotation());
  EXPECT_EQ(FootprintPad::Shape::RoundedRect, out.second->getShape());
  EXPECT_EQ(PositiveLength(3000000), out.second->getWidth());
  EXPECT_EQ(PositiveLength(4000000), out.second->getHeight());
  EXPECT_EQ(FootprintPad::ComponentSide::Top, out.second->getComponentSide());
  EXPECT_EQ(0, out.second->getHoles().count());
}

TEST_F(EagleTypeConverterTest, testConvertSmtPadRotated) {
  QString xml =
      "<smd name=\"P$1\" x=\"1\" y=\"2\" dx=\"3\" dy=\"4\" layer=\"16\" "
      "rot=\"R90\"/>";
  auto out = C::convertSmtPad(parseagle::SmtPad(dom(xml)));
  EXPECT_EQ("1", out.first->getName()->toStdString());
  EXPECT_EQ(out.first->getUuid(), out.second->getPackagePadUuid());
  EXPECT_EQ(Point(1000000, 2000000), out.second->getPosition());
  EXPECT_EQ(Angle(90000000), out.second->getRotation());
  EXPECT_EQ(FootprintPad::Shape::RoundedRect, out.second->getShape());
  EXPECT_EQ(PositiveLength(3000000), out.second->getWidth());
  EXPECT_EQ(PositiveLength(4000000), out.second->getHeight());
  EXPECT_EQ(FootprintPad::ComponentSide::Bottom,
            out.second->getComponentSide());
  EXPECT_EQ(0, out.second->getHoles().count());
}

/*******************************************************************************
 *  End of File
 ******************************************************************************/

}  // namespace tests
}  // namespace eagleimport
}  // namespace librepcb
